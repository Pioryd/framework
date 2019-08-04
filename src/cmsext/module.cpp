#include "module.h"

#include <framework/core/application.h>
#include <framework/global.h>

#include <framework/pymodule/manager.h>
#include <pybind11/embed.h>

namespace cmsext::module {
void load(const cms::string& module_name) {
  FW::G::PyModuleManager->loadAndImportModule(module_name);
}

void reload(const cms::string& module_name) {
  FW::G::PyModuleManager->reloadModule(module_name);
}

void restartModules() { FW::G::PyModuleManager->restartModules(); }

void run_script(const cms::string& script) {
  FW::G::PyModuleManager->run_script(script);
}

PYBIND11_EMBEDDED_MODULE(cmsext_module, m) {
  m.def("load", &cmsext::module::load),  //
      m.def("reload", &cmsext::module::reload),
      m.def("restartModules", &cmsext::module::restartModules),
      m.def("connect", &cmsext::module::restartModules),
      m.def("disconnect", &cmsext::module::restartModules),
      m.def(
          "connect",
          [](const std::string& signal, pybind11::object function,
             pybind11::object* globalVariable = nullptr) {
            FW::G::PyModuleManager->connect(signal, function, globalVariable);
          },
          pybind11::arg("signal"), pybind11::arg("object"),
          pybind11::arg("globalVariable") = (pybind11::object*)nullptr),

      m.def(
          "disconnect",
          [](const std::string& signal, pybind11::object function,
             pybind11::object* globalVariable = nullptr) {
            FW::G::PyModuleManager->disconnect(signal, function,
                                               globalVariable);
          },
          pybind11::arg("signal"), pybind11::arg("object"),
          pybind11::arg("globalVariable") = (pybind11::object*)nullptr);
}
}  // namespace cmsext::module