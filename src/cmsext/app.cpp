#include "app.h"

#include <framework/core/application.h>
#include <framework/global.h>

#include <framework/pymodule/manager.h>
#include <pybind11/embed.h>

namespace cmsext::app {
void close() { FW::G::Application->close(); }

cms::string get_compile_info() { return FW::G::Application->getCompileInfo(); }

cms::string get_assets_path() {
  return FW::G::Application->getAssetsPath().string();
}

cms::string get_working_directory() {
  return std::filesystem::path(FW::G::Application->getWorkingDirectory())
      .string();
}

cms::string get_type() {
  return FW::G::Application->get_type() == FW::Core::Application::Type::Console
             ? "console"
             : "graphical";
}

bool is_running() { return FW::G::Application->isRunning(); }
}  // namespace cmsext::app

PYBIND11_EMBEDDED_MODULE(cmsext_app, m) {
  m.def("close", cmsext::app::close),  //
      m.def("get_compile_info", cmsext::app::get_compile_info),
      m.def("get_assets_path", cmsext::app::get_assets_path),
      m.def("get_working_directory", cmsext::app::get_working_directory),
      m.def("is_running", cmsext::app::is_running);
}