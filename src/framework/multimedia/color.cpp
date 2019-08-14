#include "color.h"

namespace FW::MM {
const Color Color::Transparent(0, 0, 0, 0);
const Color Color::Black(0, 0, 0);
const Color Color::Navy(0, 0, 128);
const Color Color::Blue(0, 0, 255);
const Color Color::Green(0, 128, 0);
const Color Color::Teal(0, 128, 128);
const Color Color::Lime(0, 255, 0);
const Color Color::Aqua(0, 255, 255);
const Color Color::Maroon(128, 0, 0);
const Color Color::Purple(128, 0, 128);
const Color Color::Olive(128, 128, 0);
const Color Color::Gray(128, 128, 128);
const Color Color::Silver(192, 192, 192);
const Color Color::Red(255, 0, 0);
const Color Color::Fuchsia(255, 0, 255);
const Color Color::Yellow(255, 255, 0);
const Color Color::White(255, 255, 255);
const Color Color::Orange(255, 165, 0);

Color::Color()
    : r{Color::White.r},
      g{Color::White.g},
      b{Color::White.b},
      a{Color::White.a} {}

Color::Color(uint32_t rgba) { set_rgba(rgba); }

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r{r}, g{g}, b{b}, a{a} {}

Color::Color(const sf::Color& object)
    : r{object.r}, g{object.g}, b{object.b}, a{object.a} {}

uint32_t Color::get_rgba() const {
  return uint32_t(a | b << 8 | g << 16 | r << 24);
}

void Color::set_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  this->r = r;
  this->g = g;
  this->b = b;
  this->a = a;
}

void Color::set_rgba(uint32_t rgba) {
  set_rgba((rgba >> 24) & MAX_RGBA_VALUE, (rgba >> 16) & MAX_RGBA_VALUE,
          (rgba >> 8) & MAX_RGBA_VALUE, (rgba >> 0) & MAX_RGBA_VALUE);
}

bool Color::is_visible() { return (a > 0); }
}  // namespace FW::MM