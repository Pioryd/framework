#include "window.h"

#include "../global.h"

namespace FW::MM {
void Window::init(MM::SizeInt size, const std::string& name, bool fullScreen,
                  uint32_t fpsLimit) {}

void Window::poll() {}

void Window::draw(const sf::Drawable& drawable) {}

void Window::draw(const Color& color, const RectInt& rect) {}

void Window::draw(const Color& color, const RectInt& rect,
                  const Texture_ptr texture, const RectInt& textureRect) {}

void Window::clear(FW::MM::Color color) {}

void Window::display() {}

bool Window::isOpen() { return false; }

SizeInt Window::getSize() { return SizeInt(); }
}  // namespace FW::MM