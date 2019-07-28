#include "window.h"

#include "../global.h"

namespace FW::MM {
void Window::init(MM::SizeInt size, const std::string& name, bool fullScreen,
                  uint32_t fpsLimit) {
  window = std::make_unique<sf::RenderWindow>(
      sf::VideoMode(size.getWidth(), size.getHeight()), name,
      fullScreen ? sf::Style::Default | sf::Style::Fullscreen
                 : sf::Style::Default);

  window->setFramerateLimit(fpsLimit);

  gui = std::make_unique<tgui::Gui>(*window);
}

void Window::poll() {
  sf::Event event;
  while (window->pollEvent(event)) {
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed) window->close();

    if (event.type == sf::Event::Resized)
      window->setView(
          sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));

    gui->handleEvent(event);
  }
}

void Window::draw(const sf::Drawable& drawable) { window->draw(drawable); }

void Window::draw(const Color& color, const RectInt& rect) {
  sf::RectangleShape rectangle;
  rectangle.setFillColor(color);
  rectangle.setSize(rect.getSize());
  rectangle.setPosition(rect.getTopLeft());
  window->draw(rectangle);
}

void Window::draw(const Color& color, const RectInt& rect,
                  const Texture_ptr texture, const RectInt& textureRect) {
  sf::RectangleShape rectangle;
  rectangle.setTexture(texture.get());
  rectangle.setTextureRect(textureRect);
  rectangle.setFillColor(color);
  rectangle.setSize(rect.getSize());
  rectangle.setPosition(rect.getTopLeft());
  window->draw(rectangle);
}

void Window::clear(FW::MM::Color color) { window->clear(color); }

void Window::display() { window->display(); }

bool Window::isOpen() { return window->isOpen(); }

SizeInt Window::getSize() {
  // Safe cast, becouse windows size will not bigger then int32_t::max.
  return SizeInt{static_cast<int32_t>(window->getSize().x),
                 static_cast<int32_t>(window->getSize().y)};
}
}  // namespace FW::MM