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
      : x1(x), y1(y), x2(x + size.get_width()), y2(y + size.get_height()) {}
  Rect(const Point<T>& top_left, const Size<T>& size)
      : x1(top_left.x),
        y1(top_left.y),
        x2(x1 + size.get_width()),
        y2(y1 + size.get_height()) {}
  Rect(const Point<T>& top_left, const Point<T>& bottom_right)
      : x1(top_left.x), y1(top_left.y), x2(bottom_right.x), y2(bottom_right.y) {}

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
    return sf::Rect<T>(get_left(), get_top(), get_width(), get_height());
  }
  operator const sf::Rect<T>() {
    return sf::Rect<T>(get_left(), get_top(), get_width(), get_height());
  }

  bool is_valid() const { return x1 < x2 && y1 < y2; }

  T get_x() const { return x1; }
  T get_y() const { return y1; }
  T get_width() const { return x2 - x1; }
  T get_height() const { return y2 - y1; }
  Size<T> get_size() const { return Size<T>(get_width(), get_height()); }

  T get_left() const { return x1; }
  T get_top() const { return y1; }
  T get_right() const { return x2; }
  T get_bottom() const { return y2; }

  Point<T> get_top_left() const { return Point<T>(x1, y1); }
  Point<T> get_top_right() const { return Point<T>(x2, y1); }
  Point<T> get_bottom_left() const { return Point<T>(x1, y2); }
  Point<T> get_bottom_right() const { return Point<T>(x2, y2); }

  Point<T> get_center() const { return Point<T>((x1 + x2) / 2, (y1 + y2) / 2); }
  Point<T> get_top_center() const { return Point<T>((x1 + x2) / 2, y1); }
  Point<T> get_bottom_center() const { return Point<T>((x1 + x2) / 2, y2); }
  Point<T> get_center_left() const { return Point<T>(x1, (y1 + y2) / 2); }
  Point<T> get_center_right() const { return Point<T>(x2, (y1 + y2) / 2); }
  T get_horizontal_distance_to_center() const { return x1 + (x2 - x1) / 2; }
  T get_vertical_distance_to_center() const { return y1 + (y2 - y1) / 2; }

  void set_rect(T x, T y, Size<T> size) {
    x1 = x;
    y1 = y;
    x2 = (x + size.x);
    y2 = (y + size.y);
  }
  void set_rect(int left, int top, int right, int bottom) {
    x1 = left;
    y1 = top;
    x2 = right;
    y2 = bottom;
  }

  void set_x(T x) { x1 = x; }
  void set_y(T y) { y1 = y; }
  void set_width(T width) { x2 = x1 + width; }
  void set_height(T height) { y2 = y1 + height; }
  void set_size(const Size<T>& size) {
    set_size(size.get_width(), size.get_height());
  }
  void set_size(T width, T height) {
    x2 = x1 + width;
    y2 = y1 + height;
  }

  void set_left(T left) { x1 = left; }
  void set_top(T top) { y1 = top; }
  void set_right(T right) { x2 = right; }
  void set_bottom(T bottom) { y2 = bottom; }

  void set_top_left(const Point<T>& point) {
    x1 = point.x;
    y1 = point.y;
  }
  void set_top_right(const Point<T>& point) {
    x2 = point.x;
    y1 = point.y;
  }
  void set_bottom_left(const Point<T>& point) {
    x1 = point.x;
    y2 = point.y;
  }
  void set_bottom_right(const Point<T>& point) {
    x2 = point.x;
    y2 = point.y;
  }

  void expand_to_left(T value) { x1 -= value; }
  void expand_to_top(T value) { y1 -= value; }
  void expand_to_right(T value) { x2 += value; }
  void expand_to_bottom(T value) { y2 += value; }
  void expand_all(T value_left, T value_top, T value_right, T value_bottom) {
    x1 -= value_left;
    y1 -= value_top;
    x2 += value_right;
    y2 += value_bottom;
  }
  void expand_all(T value) {
    x1 -= value;
    y1 -= value;
    x2 += value;
    y2 += value;
  }

  void move_by_distance(T x, T y) {
    x1 += x;
    y1 += y;
    x2 += x;
    y2 += y;
  }
  void move_by_distance(const Point<T>& point) {
    x1 += point.x;
    y1 += point.y;
    x2 += point.x;
    y2 += point.y;
  }

  void move_to(T x, T y) {
    x2 += x - x1;
    y2 += y - y1;
    x1 = x;
    y1 = y;
  }
  void move_to(const Point<T>& point) { move_to(point.x, point.y); }
  void move_to_left(T left) {
    x2 += (left - x1);
    x1 = left;
  }
  void move_to_top(T top) {
    y2 += (top - y1);
    y1 = top;
  }
  void move_to_right(T right) {
    x1 += (right - x2);
    x2 = right;
  }
  void move_to_bottom(T bottom) {
    y1 += (bottom - y2);
    y2 = bottom;
  }
  void move_to_center(const Point<T>& point) {
    T width = x2 - x1;
    T height = y2 - y1;
    x1 = point.x - width / 2;
    y1 = point.y - height / 2;
    x2 = x1 + width;
    y2 = y1 + height;
  }
  void move_to_horizontal_center(T x) {
    T width = x2 - x1;
    x1 = x - width / 2;
    x2 = x1 + width;
  }
  void move_to_vertical_center(T y) {
    T height = y2 - y1;
    y1 = y - height / 2;
    y2 = y1 + height;
  }

  void move_to_top_left(const Point<T>& point) {
    move_to_top(point.y);
    move_to_left(point.x);
  }
  void move_to_top_right(const Point<T>& point) {
    move_to_top(point.y);
    move_to_right(point.x);
  }
  void move_to_bottom_left(const Point<T>& point) {
    move_to_bottom(point.y);
    move_to_left(point.x);
  }
  void move_to_bottom_right(const Point<T>& point) {
    move_to_bottom(point.y);
    move_to_right(point.x);
  }
  void move_to_top_center(const Point<T>& point) {
    move_to_top(point.y);
    move_to_horizontal_center(point.x);
  }
  void move_to_bottom_center(const Point<T>& point) {
    move_to_bottom(point.y);
    move_to_horizontal_center(point.x);
  }
  void move_to_left_center(const Point<T>& point) {
    move_to_left(point.x);
    move_to_vertical_center(point.y);
  }
  void move_to_right_center(const Point<T>& point) {
    move_to_right(point.x);
    move_to_vertical_center(point.y);
  }

  // Check if constains given point.
  bool contains(T x, T y) const {
    if (x < std::min(x1, static_cast<T>(x2))) return false;
    if (y < std::min(y1, static_cast<T>(y2))) return false;
    if (x >= std::max(x1, static_cast<T>(x2))) return false;
    if (y >= std::max(y1, static_cast<T>(y2))) return false;
    return true;
  }
  // Check if constains given point.
  bool contains(const Point<T>& point) const {
    return contains(point.x, point.y);
  }
  // Check if constains given rect.
  bool contains(const Rect<T>& rect) const {
    if (contains(rect.get_top_left()) && contains(rect.get_bottom_right()))
      return true;
    return false;
  }

  // Return true if is intersected with given rect.
  bool intersects(const Rect<T>& rect) const {
    if (rect.get_left() < std::min(x1, static_cast<T>(x2))) return true;
    if (rect.get_top() < std::min(y1, static_cast<T>(y2))) return true;
    if (rect.get_left() >= std::max(x1, static_cast<T>(x2))) return true;
    if (rect.get_top() >= std::max(y1, static_cast<T>(y2))) return true;

    if (rect.get_right() < std::min(x1, static_cast<T>(x2))) return true;
    if (rect.get_bottom() < std::min(y1, static_cast<T>(y2))) return true;
    if (rect.get_right() >= std::max(x1, static_cast<T>(x2))) return true;
    if (rect.get_bottom() >= std::max(y1, static_cast<T>(y2))) return true;

    return false;
  }

  // Return intersected rect.
  Rect<T> intersection(const Rect<T>& rect) const {
    if (!is_valid() || !rect.is_valid()) return Rect<T>();

    Rect<T> intersected_rect = *this;

    if (contains(rect.get_top_left()))
      intersected_rect.set_top_left(rect.get_top_left());
    if (contains(rect.get_top_right()))
      intersected_rect.set_top_right(rect.get_top_right());
    if (contains(rect.get_bottom_left()))
      intersected_rect.set_bottom_left(rect.get_bottom_left());
    if (contains(rect.get_bottom_right()))
      intersected_rect.set_bottom_right(rect.get_bottom_right());

    return intersected_rect;
  }

  // Is moving from bootom-right to top-left. Event if given rect is smaller,
  // will try to clip to top-left.
  void move_to_inside(const Rect<T>& rect) {
    if (!is_valid() || !rect.is_valid()) return;

    if (get_bottom() > rect.get_bottom()) move_to_bottom(rect.get_bottom());
    if (get_right() > rect.get_right()) move_to_right(rect.get_right());
    if (get_top() < rect.get_top()) move_to_top(rect.get_top());
    if (get_left() < rect.get_left()) move_to_left(rect.get_left());
  }

  // Return true if all cordinates are equal.
  bool operator==(const Rect<T>& object) const {
    return (x1 == object.x1 && y1 == object.y1 && x2 == object.x2 &&
            y2 == object.y2);
  }
  // Return true if one or more cordinates are not equal.
  bool operator!=(const Rect<T>& object) const {
    return (x1 != object.x1 || y1 != object.y1 || x2 != object.x2 ||
            y2 != object.y2);
  }

 private:
  T x1, y1, x2, y2;
};

typedef Rect<uint32_t> RectUInt;
typedef Rect<int32_t> RectInt;
typedef Rect<float> RectFloat;

}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_RECT_H
