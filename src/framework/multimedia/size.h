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
  Size(T newWidth, T newHeight) : width{newWidth}, height{newHeight} {
    valid();
  };

  // After set, value is valid().
  Size(const Size<T>& object)
      : width{object.getWidth()}, height{object.getHeight()} {
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
    throwIfCantDivideOrMultiply(value);
    return Size<T>((T)width * value, (T)height * value);
  }
  // Throw if value is less or equal to 0;
  Size<T>& operator*=(float value) {
    throwIfCantDivideOrMultiply(value);
    width = (T)width * value;
    height = (T)height * value;
    return *this;
  }
  // Throw if value is less or equal to 0;
  Size<T> operator/(float value) const {
    throwIfCantDivideOrMultiply(value);
    return Size<T>((T)width / value, (T)height / value);
  }
  // Throw if value is less or equal to 0;
  Size<T>& operator/=(float value) {
    throwIfCantDivideOrMultiply(value);
    width /= value;
    height /= value;
    return *this;
  }

  // Compare areas
  bool operator<=(const Size<T>& object) const {
    return getArea() <= object.getArea();
  }
  // Compare areas
  bool operator>=(const Size<T>& object) const {
    return getArea() >= object.getArea();
  }
  // Compare areas
  bool operator<(const Size<T>& object) const {
    return getArea() < object.getArea();
  }
  // Compare areas
  bool operator>(const Size<T>& object) const {
    return getArea() > object.getArea();
  }

  // Compare areas
  bool operator==(const Size<T>& object) const {
    return getArea() == object.getArea();
  }
  // Compare areas
  bool operator!=(const Size<T>& object) const {
    return getArea() != object.getArea();
  }

  int getWidth() const { return width; }
  int getHeight() const { return height; }

  T getArea() const { return width * height; }

  // After set, value is valid().
  void setWidth(T newWidth) {
    width = newWidth;
    valid();
  }
  // After set, value is valid().
  void setHeight(T newHeight) {
    height = newHeight;
    valid();
  }

  // After set, value is valid().
  void resize(T newWidth, T newHeight, bool keepAspectRatio = false) {
    if (keepAspectRatio == false) {
      width = newWidth;
      height = newHeight;
    } else {
      T scaledWidthRatio = (newHeight * width) / height;
      T scaledHeightRatio = (newWidth * height) / width;

      if (scaledWidthRatio <= newWidth) {
        width = scaledWidthRatio;
        height = newHeight;
      } else {
        width = newWidth;
        height = scaledHeightRatio;
      }
    }
    valid();
  }
  // After set, value is valid().
  void resize(const Size<T>& size, bool keepAspectRatio = false) {
    resize(size.width, size.height, keepAspectRatio);
  }

  // Check if width and height is greater or equal to 0;
  bool isValid() const { return width >= 0 && height >= 0; }

 protected:
  // If width or height is less then "0", then set to "0".
  void valid() {
    if (width < 0) width = 0;
    if (height < 0) height = 0;
  }
  template <typename V>
  // Throw if value is less or equal to "0".
  void throwIfCantDivideOrMultiply(V value) {
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
