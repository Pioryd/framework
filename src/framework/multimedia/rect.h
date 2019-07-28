#ifndef FW_MULTIMEDIA_RECT_H
#define FW_MULTIMEDIA_RECT_H

// Can't include, becouse current file is included in declarations.
//#include "declarations.h"

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
  Rect<T>& operator=(const Rect<T>& other) {
    x1 = other.x1;
    y1 = other.y1;
    x2 = other.x2;
    y2 = other.y2;
    return *this;
  }

  operator sf::Rect<T>() const {
    return sf::Rect<T>(getLeft(), getTop(), getWidth(), getHeight());
  }
  operator const sf::Rect<T>() {
    return sf::Rect<T>(getLeft(), getTop(), getWidth(), getHeight());
  }

  bool isValid() const { return x1 < x2 && y1 < y2; }

  T getX() const { return x1; }
  T getY() const { return y1; }
  T getWidth() const { return x2 - x1; }
  T getHeight() const { return y2 - y1; }
  Size<T> getSize() const { return Size<T>(getWidth(), getHeight()); }

  T getLeft() const { return x1; }
  T getTop() const { return y1; }
  T getRight() const { return x2; }
  T getBottom() const { return y2; }

  Point<T> getTopLeft() const { return Point<T>(x1, y1); }
  Point<T> getTopRight() const { return Point<T>(x2, y1); }
  Point<T> getBottomLeft() const { return Point<T>(x1, y2); }
  Point<T> getBottomRight() const { return Point<T>(x2, y2); }

  Point<T> getCenter() const { return Point<T>((x1 + x2) / 2, (y1 + y2) / 2); }
  Point<T> getTopCenter() const { return Point<T>((x1 + x2) / 2, y1); }
  Point<T> getBottomCenter() const { return Point<T>((x1 + x2) / 2, y2); }
  Point<T> getCenterLeft() const { return Point<T>(x1, (y1 + y2) / 2); }
  Point<T> getCenterRight() const { return Point<T>(x2, (y1 + y2) / 2); }
  T getHorizontalDistanceToCenter() const { return x1 + (x2 - x1) / 2; }
  T getVerticalDistanceToCenter() const { return y1 + (y2 - y1) / 2; }

  void setRect(T x, T y, Size<T> size) {
    x1 = x;
    y1 = y;
    x2 = (x + size.x);
    y2 = (y + size.y);
  }
  void setRect(int left, int top, int right, int bottom) {
    x1 = left;
    y1 = top;
    x2 = right;
    y2 = bottom;
  }

  void setX(T x) { x1 = x; }
  void setY(T y) { y1 = y; }
  void setWidth(T width) { x2 = x1 + width; }
  void setHeight(T height) { y2 = y1 + height; }
  void setSize(const Size<T>& size) {
    setSize(size.getWidth(), size.getHeight());
  }
  void setSize(T width, T height) {
    x2 = x1 + width;
    y2 = y1 + height;
  }

  void setLeft(T left) { x1 = left; }
  void setTop(T top) { y1 = top; }
  void setRight(T right) { x2 = right; }
  void setBottom(T bottom) { y2 = bottom; }

  void setTopLeft(const Point<T>& point) {
    x1 = point.x;
    y1 = point.y;
  }
  void setTopRight(const Point<T>& point) {
    x2 = point.x;
    y1 = point.y;
  }
  void setBottomLeft(const Point<T>& point) {
    x1 = point.x;
    y2 = point.y;
  }
  void setBottomRight(const Point<T>& point) {
    x2 = point.x;
    y2 = point.y;
  }

  void expandToLeft(T value) { x1 -= value; }
  void expandToTop(T value) { y1 -= value; }
  void expandToRight(T value) { x2 += value; }
  void expandToBottom(T value) { y2 += value; }
  void expandAll(T valueLeft, T valueTop, T valueRight, T valueBottom) {
    x1 -= valueLeft;
    y1 -= valueTop;
    x2 += valueRight;
    y2 += valueBottom;
  }
  void expandAll(T value) {
    x1 -= value;
    y1 -= value;
    x2 += value;
    y2 += value;
  }

  void moveByDistance(T x, T y) {
    x1 += x;
    y1 += y;
    x2 += x;
    y2 += y;
  }
  void moveByDistance(const Point<T>& point) {
    x1 += point.x;
    y1 += point.y;
    x2 += point.x;
    y2 += point.y;
  }

  void moveTo(T x, T y) {
    x2 += x - x1;
    y2 += y - y1;
    x1 = x;
    y1 = y;
  }
  void moveTo(const Point<T>& point) { moveTo(point.x, point.y); }
  void moveToLeft(T left) {
    x2 += (left - x1);
    x1 = left;
  }
  void moveToTop(T top) {
    y2 += (top - y1);
    y1 = top;
  }
  void moveToRight(T right) {
    x1 += (right - x2);
    x2 = right;
  }
  void moveToBottom(T bottom) {
    y1 += (bottom - y2);
    y2 = bottom;
  }
  void moveToCenter(const Point<T>& point) {
    T width = x2 - x1;
    T height = y2 - y1;
    x1 = point.x - width / 2;
    y1 = point.y - height / 2;
    x2 = x1 + width;
    y2 = y1 + height;
  }
  void moveToHorizontalCenter(T x) {
    T width = x2 - x1;
    x1 = x - width / 2;
    x2 = x1 + width;
  }
  void moveToVerticalCenter(T y) {
    T height = y2 - y1;
    y1 = y - height / 2;
    y2 = y1 + height;
  }

  void moveToTopLeft(const Point<T>& point) {
    moveToTop(point.y);
    moveToLeft(point.x);
  }
  void moveToTopRight(const Point<T>& point) {
    moveToTop(point.y);
    moveToRight(point.x);
  }
  void moveToBottomLeft(const Point<T>& point) {
    moveToBottom(point.y);
    moveToLeft(point.x);
  }
  void moveToBottomRight(const Point<T>& point) {
    moveToBottom(point.y);
    moveToRight(point.x);
  }
  void moveToTopCenter(const Point<T>& point) {
    moveToTop(point.y);
    moveToHorizontalCenter(point.x);
  }
  void moveToBottomCenter(const Point<T>& point) {
    moveToBottom(point.y);
    moveToHorizontalCenter(point.x);
  }
  void moveToLeftCenter(const Point<T>& point) {
    moveToLeft(point.x);
    moveToVerticalCenter(point.y);
  }
  void moveToRightCenter(const Point<T>& point) {
    moveToRight(point.x);
    moveToVerticalCenter(point.y);
  }

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
