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
  Size() : width{0}, height{0} {};
  // After set, value is valid().
  Size(T newWidth, T newHeight)
      : width{newWidth},
        height{newHeight} {

        };

  // After set, value is valid().
  Size(const Size<T>& object)
      : width{object.getWidth()},
        height{object.getHeight()} {

        };
  // After set, value is valid().
  Size<T>& operator=(const Size<T>& object) { return nullptr; }

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

  // After set, value is valid().
  Size<T> operator+(const Size<T>& object) const { return Size<T>(); }
  // After set, value is valid().
  Size<T> operator-(const Size<T>& object) const { return Size<T>(); }

  // After set, value is valid().
  Size<T>& operator+=(const Size<T>& object) { return nullptr; }
  // After set, value is valid().
  Size<T>& operator-=(const Size<T>& object) { return nullptr; }

  // Throw if value is less or equal to 0;
  Size<T> operator*(float value) const { return Size<T>(); }
  // Throw if value is less or equal to 0;
  Size<T>& operator*=(float value) { return nullptr; }
  // Throw if value is less or equal to 0;
  Size<T> operator/(float value) const { return Size<T>(); }
  // Throw if value is less or equal to 0;
  Size<T>& operator/=(float value) { return nullptr; }

  // Compare areas
  bool operator<=(const Size<T>& object) const { return false; }
  // Compare areas
  bool operator>=(const Size<T>& object) const { return false; }
  // Compare areas
  bool operator<(const Size<T>& object) const { return false; }
  // Compare areas
  bool operator>(const Size<T>& object) const { return false; }

  // Compare areas
  bool operator==(const Size<T>& object) const { return false; }
  // Compare areas
  bool operator!=(const Size<T>& object) const { return false; }

  int getWidth() const { return 0; }
  int getHeight() const { return 0; }

  T getArea() const { return T(); }

  // After set, value is valid().
  void setWidth(T newWidth) {}
  // After set, value is valid().
  void setHeight(T newHeight) {}

  // After set, value is valid().
  void resize(T newWidth, T newHeight, bool keepAspectRatio = false) {}
  // After set, value is valid().
  void resize(const Size<T>& size, bool keepAspectRatio = false) {}

  // Check if width and height is greater or equal to 0;
  bool isValid() const { return false; }

 protected:
  // If width or height is less then "0", then set to "0".
  void valid() {}
  template <typename V>
  // Throw if value is less or equal to "0".
  void throwIfCantDivideOrMultiply(V value) {}

 private:
  T width;
  T height;
};  // namespace FW::Type

typedef Size<uint32_t> SizeUInt;
typedef Size<int32_t> SizeInt;
typedef Size<float> SizeFloat;
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_SIZE_H
