#ifndef FW_MULTIMEDIA_RECT_H
#define FW_MULTIMEDIA_RECT_H

#include <SFML/Graphics.hpp>
#include <type_traits>
#include "../pch.h"

namespace FW::MM {
template <typename T>
class Point;

template <typename T>
class Size;

template <typename T>
class Rect {
 public:
  Rect() : x1(0), y1(0), x2(0), y2(0) {}
  Rect(T x, T y, T width, T height)
      : x1(x), y1(y), x2(x + width), y2(y + height) {}
  Rect(T x, T y, const Size<T>& size)
      : x1(x), y1(y), x2(x + size.getWidth()), y2(y + size.getHeight()) {}
  Rect(const Point<T>& topLeft, const Size<T>& size)
      : x1(topLeft.x),
        y1(topLeft.y),
        x2(x1 + size.getWidth()),
        y2(y1 + size.getHeight()) {}
  Rect(const Point<T>& topLeft, const Point<T>& bottomRight)
      : x1(topLeft.x), y1(topLeft.y), x2(bottomRight.x), y2(bottomRight.y) {}

  Rect(const Rect<T>& other)
      : x1(other.x1), y1(other.y1), x2(other.x2), y2(other.y2) {}
  Rect<T>& operator=(const Rect<T>& other) { return nullptr; }

  operator sf::Rect<T>() const { return sf::Rect<T>(); }
  operator const sf::Rect<T>() { return sf::Rect<T>(); }

  bool isValid() const { return false; }

  T getX() const { return T(); }
  T getY() const { return T(); }
  T getWidth() const { return T(); }
  T getHeight() const { return T(); }
  Size<T> getSize() const { return Size<T>(); }

  T getLeft() const { return T(); }
  T getTop() const { return T(); }
  T getRight() const { return T(); }
  T getBottom() const { return T(); }

  Point<T> getTopLeft() const { return Point < T(); }
  Point<T> getTopRight() const { return Point < T(); }
  Point<T> getBottomLeft() const { return Point < T(); }
  Point<T> getBottomRight() const { return Point < T(); }

  Point<T> getCenter() const { return Point < T(); }
  Point<T> getTopCenter() const { return Point < T(); }
  Point<T> getBottomCenter() const { return Point < T(); }
  Point<T> getCenterLeft() const { return Point < T(); }
  Point<T> getCenterRight() const { return Point < T(); }
  T getHorizontalDistanceToCenter() const { return T(); }
  T getVerticalDistanceToCenter() const { return T(); }

  void setRect(T x, T y, Size<T> size) {}
  void setRect(int left, int top, int right, int bottom) {}

  void setX(T x) {}
  void setY(T y) {}
  void setWidth(T width) {}
  void setHeight(T height) {}
  void setSize(const Size<T>& size) {}
  void setSize(T width, T height) {}

  void setLeft(T left) {}
  void setTop(T top) {}
  void setRight(T right) {}
  void setBottom(T bottom) {}

  void setTopLeft(const Point<T>& point) {}
  void setTopRight(const Point<T>& point) {}
  void setBottomLeft(const Point<T>& point) {}
  void setBottomRight(const Point<T>& point) {}

  void expandToLeft(T value) {}
  void expandToTop(T value) {}
  void expandToRight(T value) {}
  void expandToBottom(T value) {}
  void expandAll(T valueLeft, T valueTop, T valueRight, T valueBottom) {}
  void expandAll(T value) {}

  void moveByDistance(T x, T y) {}
  void moveByDistance(const Point<T>& point) {}

  void moveTo(T x, T y) {}
  void moveTo(const Point<T>& point) {}
  void moveToLeft(T left) {}
  void moveToTop(T top) {}
  void moveToRight(T right) {}
  void moveToBottom(T bottom) {}
  void moveToCenter(const Point<T>& point) {}
  void moveToHorizontalCenter(T x) {}
  void moveToVerticalCenter(T y) {}

  void moveToTopLeft(const Point<T>& point) {}
  void moveToTopRight(const Point<T>& point) {}
  void moveToBottomLeft(const Point<T>& point) {}
  void moveToBottomRight(const Point<T>& point) {}
  void moveToTopCenter(const Point<T>& point) {}
  void moveToBottomCenter(const Point<T>& point) {}
  void moveToLeftCenter(const Point<T>& point) {}
  void moveToRightCenter(const Point<T>& point) {}

  // Check if constains given point.
  bool contains(T x, T y) const { return false; }
  // Check if constains given point.
  bool contains(const Point<T>& point) const { return false; }
  // Check if constains given rect.
  bool contains(const Rect<T>& rect) const { return false; }

  // Return true if is intersected with given rect.
  bool intersects(const Rect<T>& rect) const { return false; }

  // Return intersected rect.
  Rect<T> intersection(const Rect<T>& rect) const { return Rect<T>(); }

  // Is moving from bootom-right to top-left. Event if given rect is smaller,
  // will try to clip to top-left.
  void moveToInside(const Rect<T>& rect) {}

  // Return true if all cordinates are equal.
  bool operator==(const Rect<T>& object) const { return false; }
  // Return true if one or more cordinates are not equal.
  bool operator!=(const Rect<T>& object) const { return false; }

 private:
  T x1, y1, x2, y2;
};

typedef Rect<uint32_t> RectUInt;
typedef Rect<int32_t> RectInt;
typedef Rect<float> RectFloat;

}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_RECT_H
