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
  void init(MM::SizeInt size, const std::string& name, bool full_screen,
            uint32_t fps_limit);
  void poll();

  void draw(const sf::Drawable& drawable);
  void draw(const Color& color, const RectInt& rect);
  void draw(const Color& color, const RectInt& rect, const Texture_ptr texture,
            const RectInt& texture_rect);

  void clear(FW::MM::Color color);
  void display();

  bool is_open();
  SizeInt get_size();

 public:
  std::unique_ptr<sf::RenderWindow> window;
  std::unique_ptr<tgui::Gui> gui;
  tgui::Theme theme;
};
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_WINDOW_H
