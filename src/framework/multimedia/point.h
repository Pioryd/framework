#ifndef FW_MULTIMEDIA_POINT_H
#define FW_MULTIMEDIA_POINT_H

#include <SFML/Graphics.hpp>
#include "../pch.h"

namespace FW::MM {
template <typename T>
class Point {
 public:
  Point() : x{0}, y{0} {}
  Point(T x, T y) : x{x}, y{y} {}

  Point(const Point<T>& other) : x{other.x}, y{other.y} {}
  Point<T>& operator=(const Point<T>& other) { return nullptr; }

  template <typename U, typename = typename std::enable_if<
                            std::is_arithmetic<U>::value, U>::type>
  operator sf::Vector2<U>() const {
    return sf::Vector2<U>();
  }
  template <typename U, typename = typename std::enable_if<
                            std::is_arithmetic<U>::value, U>::type>
  operator const sf::Vector2<U>() {
    return sf::Vector2<U>();
  }

  Point<T>
  // Adds x to x and y to y.
  operator+(const Point<T>& other) const {
    return Point<T>();
  }
  // Subtracts x from x and y from y.
  Point<T> operator-(const Point<T>& other) const { return Point<T>(); }

  // Add x to x and y to y.
  Point<T>& operator+=(const Point<T>& other) { return nullptr; }
  // Subtracts x from x and y from y.
  Point<T>& operator-=(const Point<T>& other) { return nullptr; }

  // True if compared x and y are same.
  bool operator==(const Point<T>& other) const { return false; }
  // True if one of the compared x and y is not the same.
  bool operator!=(const Point<T>& other) const { return false; }

  float distance(const Point& other) const { return 0; }

  T x;
  T y;
};

typedef Point<uint32_t> PointUInt;
typedef Point<int32_t> PointInt;
typedef Point<float> PointFloat;
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_POINT_H
