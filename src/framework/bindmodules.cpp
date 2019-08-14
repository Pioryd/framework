//#include "global.h"

#include "global.h"
#include "core/logger.h"

#include "pymodule/manager.h"
#include <pybind11/embed.h>

PYBIND11_EMBEDDED_MODULE(fw_module_manager, m) {
  // `m` is a `pybind11::module` which is used to bind functions and classes
  m.def("connect",
        [](const std::string& signal, pybind11::object function,
           pybind11::object* globalVariable = nullptr) {
          FW::G::PyModule_Manager->connect(signal, function, globalVariable);
        },
        "Connect to signal", pybind11::arg("signal"), pybind11::arg("object"),
        pybind11::arg("globalVariable") = (pybind11::object*)nullptr),

      m.def("disconnect",
            [](const std::string& signal, pybind11::object function,
               pybind11::object* globalVariable = nullptr) {
              FW::G::PyModule_Manager->disconnect(signal, function, globalVariable);
            },
            "Disconnect to signal", pybind11::arg("signal"), pybind11::arg("object"),
            pybind11::arg("globalVariable") = (pybind11::object*)nullptr);
}
PYBIND11_EMBEDDED_MODULE(fw_core_logger, m) {
  // `m` is a `pybind11::module` which is used to bind functions and classes
  m.def("info",
        [](const std::string& message, const std::string& function = "",
           const std::string& moduleGlobalName = "") {
          FW::G::Logger.info(message, function, moduleGlobalName);
        },
        "Print info log", pybind11::arg("message"), pybind11::arg("function") = "",
        pybind11::arg("moduleGlobalName") = ""),

      m.def("error",
            [](const std::string& message, const std::string& function = "",
               const std::string& moduleGlobalName = "") {
              FW::G::Logger.info(message, function, moduleGlobalName);
            },
            "Print error log", pybind11::arg("message"), pybind11::arg("function") = "",
            pybind11::arg("moduleGlobalName") = ""),

      m.def("debug",
            [](const std::string& message, const std::string& function = "",
               const std::string& moduleGlobalName = "") {
              FW::G::Logger.debug(message, function, moduleGlobalName);
            },
            "Print debug log", pybind11::arg("message"), pybind11::arg("function") = "",
            pybind11::arg("moduleGlobalName") = "");
}