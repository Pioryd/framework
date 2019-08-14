#ifndef FW_MULTIMEDIA_POINT_H
#define FW_MULTIMEDIA_POINT_H

// Can't include, becouse current this file is included in declarations.
//#include "declarations.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../pch.h"

namespace FW::MM {
// Dont use type bigger then float. Becouse in many cases its usesed can be cast
// to sf::Vector2<float>
template <typename T>
class Point {
 public:
  Point() : x{0}, y{0} {}
  Point(T x, T y) : x{x}, y{y} {}

  Point(const Point<T>& other) : x{other.x}, y{other.y} {}
  Point<T>& operator=(const Point<T>& other) {
    x = other.x;
    y = other.y;
    return *this;
  }

  template <typename U, typename = typename std::enable_if<
                            std::is_arithmetic<U>::value, U>::type>
  operator sf::Vector2<U>() const {
    return sf::Vector2<U>((U)x, (U)y);
  }
  template <typename U, typename = typename std::enable_if<
                            std::is_arithmetic<U>::value, U>::type>
  operator const sf::Vector2<U>() {
    return sf::Vector2<U>((U)x, (U)y);
  }

  Point<T>
  // Adds x to x and y to y.
  operator+(const Point<T>& other) const {
    return Point<T>(x + other.x, y + other.y);
  }
  // Subtracts x from x and y from y.
  Point<T> operator-(const Point<T>& other) const {
    return Point<T>(x - other.x, y - other.y);
  }

  // Add x to x and y to y.
  Point<T>& operator+=(const Point<T>& other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  // Subtracts x from x and y from y.
  Point<T>& operator-=(const Point<T>& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  // True if compared x and y are same.
  bool operator==(const Point<T>& other) const {
    return other.x == x && other.y == y;
  }
  // True if one of the compared x and y is not the same.
  bool operator!=(const Point<T>& other) const {
    return other.x != x || other.y != y;
  }

  float distance(const Point& other) const {
    const float x_diff = x - other.x;
    const float y_diff = y - other.y;
    return std::sqrt(x_diff * x_diff + y_diff * y_diff);
  }

  T x;
  T y;
};

typedef Point<uint32_t> PointUInt;
typedef Point<int32_t> PointInt;
typedef Point<float> PointFloat;
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_POINT_H
