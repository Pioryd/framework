#ifndef FW_MULTIMEDIA_SIZE_H
#define FW_MULTIMEDIA_SIZE_H

// Can't include, becouse current file is included in declarations.
//#include "declarations.h"

#include <SFML/Graphics.hpp>
#include <type_traits>
#include "../pch.h"

namespace FW::MM {
// Size template can be as all numerics. But class is working as unsigned.
// Dont use type bigger then float. Becouse in many cases its usesed can be cast
// to sf::Vector2<float>
template <typename T>
class Size {
 public:
  // After set, value is valid().
  Size() : width{0}, height{0} { valid(); };
  // After set, value is valid().
  Size(T new_width, T new_height) : width{new_width}, height{new_height} {
    valid();
  };

  // After set, value is valid().
  Size(const Size<T>& object)
      : width{object.get_width()}, height{object.get_height()} {
    valid();
  };
  // After set, value is valid().
  Size<T>& operator=(const Size<T>& object) {
    width = object.width;
    height = object.height;
    valid();
    return *this;
  }

  template <typename U, typename = typename std::enable_if<
                            std::is_arithmetic<U>::value, U>::type>
  operator sf::Vector2<U>() const {
    return sf::Vector2<U>((U)width, (U)height);
  }
  template <typename U, typename = typename std::enable_if<
                            std::is_arithmetic<U>::value, U>::type>
  operator const sf::Vector2<U>() {
    return sf::Vector2<U>((U)width, (U)height);
  }

  // After set, value is valid().
  Size<T> operator+(const Size<T>& object) const {
    // valid() inside constructor
    return Size<T>(width + object.width, height + object.height);
  }
  // After set, value is valid().
  Size<T> operator-(const Size<T>& object) const {
    // valid() inside constructor
    return Size<T>(width - object.width, height - object.height);
  }

  // After set, value is valid().
  Size<T>& operator+=(const Size<T>& object) {
    width += object.width;
    height += object.height;
    valid();
    return *this;
  }
  // After set, value is valid().
  Size<T>& operator-=(const Size<T>& object) {
    width -= object.width;
    height -= object.height;
    valid();
    return *this;
  }

  // Throw if value is less or equal to 0;
  Size<T> operator*(float value) const {
    throw_if_cant_divide_or_multiply(value);
    return Size<T>((T)width * value, (T)height * value);
  }
  // Throw if value is less or equal to 0;
  Size<T>& operator*=(float value) {
    throw_if_cant_divide_or_multiply(value);
    width = (T)width * value;
    height = (T)height * value;
    return *this;
  }
  // Throw if value is less or equal to 0;
  Size<T> operator/(float value) const {
    throw_if_cant_divide_or_multiply(value);
    return Size<T>((T)width / value, (T)height / value);
  }
  // Throw if value is less or equal to 0;
  Size<T>& operator/=(float value) {
    throw_if_cant_divide_or_multiply(value);
    width /= value;
    height /= value;
    return *this;
  }

  // Compare areas
  bool operator<=(const Size<T>& object) const {
    return get_area() <= object.get_area();
  }
  // Compare areas
  bool operator>=(const Size<T>& object) const {
    return get_area() >= object.get_area();
  }
  // Compare areas
  bool operator<(const Size<T>& object) const {
    return get_area() < object.get_area();
  }
  // Compare areas
  bool operator>(const Size<T>& object) const {
    return get_area() > object.get_area();
  }

  // Compare areas
  bool operator==(const Size<T>& object) const {
    return get_area() == object.get_area();
  }
  // Compare areas
  bool operator!=(const Size<T>& object) const {
    return get_area() != object.get_area();
  }

  int get_width() const { return width; }
  int get_height() const { return height; }

  T get_area() const { return width * height; }

  // After set, value is valid().
  void set_width(T new_width) {
    width = new_width;
    valid();
  }
  // After set, value is valid().
  void set_height(T new_height) {
    height = new_height;
    valid();
  }

  // After set, value is valid().
  void resize(T new_width, T new_height, bool keep_aspect_ratio = false) {
    if (keep_aspect_ratio == false) {
      width = new_width;
      height = new_height;
    } else {
      T scaled_width_ratio = (new_height * width) / height;
      T scaled_height_ratio = (new_width * height) / width;

      if (scaled_width_ratio <= new_width) {
        width = scaled_width_ratio;
        height = new_height;
      } else {
        width = new_width;
        height = scaled_height_ratio;
      }
    }
    valid();
  }
  // After set, value is valid().
  void resize(const Size<T>& size, bool keep_aspect_ratio = false) {
    resize(size.width, size.height, keep_aspect_ratio);
  }

  // Check if width and height is greater or equal to 0;
  bool is_valid() const { return width >= 0 && height >= 0; }

 protected:
  // If width or height is less then "0", then set to "0".
  void valid() {
    if (width < 0) width = 0;
    if (height < 0) height = 0;
  }
  template <typename V>
  // Throw if value is less or equal to "0".
  void throw_if_cant_divide_or_multiply(V value) {
    if (value <= 0)
      throw std::runtime_error("Value[" + std::to_string(value) +
                               "] is wrong. Can't divide or multiply.");
  }

 private:
  T width;
  T height;
};  // namespace FW::Type

typedef Size<uint32_t> SizeUInt;
typedef Size<int32_t> SizeInt;
typedef Size<float> SizeFloat;
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_SIZE_H
