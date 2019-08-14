#include "module.h"

#include <framework/core/application.h>
#include <framework/global.h>

#include <framework/pymodule/manager.h>
#include <pybind11/embed.h>

namespace cmsext::module {
void load(const cms::string& module_name) {
  FW::G::PyModule_Manager->load_and_import_module(module_name);
}

void reload(const cms::string& module_name) {
  FW::G::PyModule_Manager->reload_module(module_name);
}

void restart_modules() { FW::G::PyModule_Manager->restart_modules(); }

void run_script(const cms::string& script) {
  FW::G::PyModule_Manager->run_script(script);
}

PYBIND11_EMBEDDED_MODULE(cmsext_module, m) {
  m.def("load", &cmsext::module::load),  //
      m.def("reload", &cmsext::module::reload),
      m.def("restart_modules", &cmsext::module::restart_modules),
      m.def("connect", &cmsext::module::restart_modules),
      m.def("disconnect", &cmsext::module::restart_modules),
      m.def(
          "connect",
          [](const std::string& signal, pybind11::object function,
             pybind11::object* globalVariable = nullptr) {
            FW::G::PyModule_Manager->connect(signal, function, globalVariable);
          },
          pybind11::arg("signal"), pybind11::arg("object"),
          pybind11::arg("globalVariable") = (pybind11::object*)nullptr),

      m.def(
          "disconnect",
          [](const std::string& signal, pybind11::object function,
             pybind11::object* globalVariable = nullptr) {
            FW::G::PyModule_Manager->disconnect(signal, function,
                                               globalVariable);
          },
          pybind11::arg("signal"), pybind11::arg("object"),
          pybind11::arg("globalVariable") = (pybind11::object*)nullptr);
}
}  // namespace cmsext::module