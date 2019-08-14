#include "window.h"

#include "../core/application.h"
#include "../global.h"

namespace FW::MM {
void Window::init(MM::SizeInt size, const std::string& name, bool full_screen,
                  uint32_t fps_limit) {
  window = std::make_unique<sf::RenderWindow>(
      sf::VideoMode(size.get_width(), size.get_height()), name,
      full_screen ? sf::Style::Default | sf::Style::Fullscreen
                 : sf::Style::Default);

  window->setFramerateLimit(fps_limit);

  gui = std::make_unique<tgui::Gui>(*window);

  theme = tgui::Theme{FW::G::Application->get_assets_path().string() +
                      "/themes/Black.txt"};
}

void Window::poll() {
  sf::Event event;
  while (window->pollEvent(event)) {
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed) window->close();

    if (event.type == sf::Event::Resized) {
      /*window->setView(
          sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));*/
      gui->setView(
          sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
    }


    gui->handleEvent(event);
  }
}

void Window::draw(const sf::Drawable& drawable) { window->draw(drawable); }

void Window::draw(const Color& color, const RectInt& rect) {
  sf::RectangleShape rectangle;
  rectangle.setFillColor(color);
  rectangle.setSize(rect.get_size());
  rectangle.setPosition(rect.get_top_left());
  window->draw(rectangle);
}

void Window::draw(const Color& color, const RectInt& rect,
                  const Texture_ptr texture, const RectInt& texture_rect) {
  sf::RectangleShape rectangle;
  rectangle.setTexture(texture.get());
  rectangle.setTextureRect(texture_rect);
  rectangle.setFillColor(color);
  rectangle.setSize(rect.get_size());
  rectangle.setPosition(rect.get_top_left());
  window->draw(rectangle);
}

void Window::clear(FW::MM::Color color) { window->clear(color); }

void Window::display() { window->display(); }

bool Window::is_open() { return window->isOpen(); }

SizeInt Window::get_size() {
  // Safe cast, becouse windows size will not bigger then int32_t::max.
  return SizeInt{static_cast<int32_t>(window->getSize().x),
                 static_cast<int32_t>(window->getSize().y)};
}
}  // namespace FW::MM