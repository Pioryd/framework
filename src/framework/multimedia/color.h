#ifndef FW_MULTIMEDIA_COLOR_H
#define FW_MULTIMEDIA_COLOR_H

#include "declarations.h"

#include <SFML/Graphics.hpp>

namespace FW::MM {
class Color {
  enum : uint8_t { MIN_RGBA_VALUE = 0, MAX_RGBA_VALUE = 255 };

 public:
  Color();
  Color(uint32_t rgba);
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = MAX_RGBA_VALUE);

  Color(const sf::Color& object);
  Color& operator=(const Color& object);

  operator sf::Color() const { return sf::Color(r, g, b, a); }

  Color operator+(const Color& object) const;
  Color operator-(const Color& object) const;

  Color operator+=(const Color& object);
  Color operator-=(const Color& object);

  Color operator*(const Color& object) const;

  Color operator*=(const Color& object);

  bool operator==(const Color& object);
  bool operator!=(const Color& object);

  uint32_t getRGBA() const;
  void setRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = MAX_RGBA_VALUE);
  void setRGBA(uint32_t rgba);

  bool isVisible();

  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  static const Color Transparent;
  static const Color White;
  static const Color Navy;
  static const Color Blue;
  static const Color Green;
  static const Color Teal;
  static const Color Lime;
  static const Color Aqua;
  static const Color Maroon;
  static const Color Purple;
  static const Color Olive;
  static const Color Gray;
  static const Color Silver;
  static const Color Red;
  static const Color Fuchsia;
  static const Color Yellow;
  static const Color Black;
  static const Color Orange;
};

inline Color& Color::operator=(const Color& object) { return Color(); }

inline Color Color::operator+(const Color& object) const { return Color(); }

inline Color Color::operator-(const Color& object) const { return Color(); }

inline Color Color::operator+=(const Color& object) { return Color(); }

inline Color Color::operator-=(const Color& object) { return Color(); }

inline Color Color::operator*(const Color& object) const { return Color(); }

inline Color Color::operator*=(const Color& object) { return Color(); }

inline bool Color::operator==(const Color& object) { return false; }

inline bool Color::operator!=(const Color& object) { return false; }
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_COLOR_H