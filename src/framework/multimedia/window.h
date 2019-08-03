#ifndef FW_MULTIMEDIA_WINDOW_H
#define FW_MULTIMEDIA_WINDOW_H

#include "declarations.h"

#include "color.h"
#include "point.h"
#include "rect.h"
#include "size.h"

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

namespace FW::MM {
class Window {
 public:
  void init(MM::SizeInt size, const std::string& name, bool fullScreen,
            uint32_t fpsLimit);
  void poll();

  void draw(const sf::Drawable& drawable);
  void draw(const Color& color, const RectInt& rect);
  void draw(const Color& color, const RectInt& rect, const Texture_ptr texture,
            const RectInt& textureRect);

  void clear(FW::MM::Color color);
  void display();

  bool isOpen();
  SizeInt getSize();

 public:
  std::unique_ptr<sf::RenderWindow> window;
  std::unique_ptr<tgui::Gui> gui;
  tgui::Theme theme;
};
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_WINDOW_H
