#include "gui.h"

#include <framework/global.h>
#include <framework/multimedia/window.h>

#include <framework/pymodule/manager.h>
#include <pybind11/embed.h>

namespace cmsext::gui {
void set_window_size(int32_t width, int32_t height) {
  FW::G::Window.window->setSize(sf::Vector2u(width, height));
}
int32_t get_window_width() { return FW::G::Window.window->getSize().x; }
int32_t get_window_height() { return FW::G::Window.window->getSize().y; }
}  // namespace cmsext::gui

PYBIND11_EMBEDDED_MODULE(cmsext_gui, m) {
  m.def("set_window_size", cmsext::gui::set_window_size),  //
      m.def("get_window_width", cmsext::gui::get_window_width),
      m.def("get_window_height", cmsext::gui::get_window_height);
}