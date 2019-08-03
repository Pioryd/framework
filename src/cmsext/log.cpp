#include "log.h"

#include <framework/core/logger.h>
#include <framework/global.h>

#include <framework/pymodule/manager.h>
#include <pybind11/embed.h>

namespace cmsext::log {
void debug(const cms::string& what) { FW::G::Logger.debug(what); }
void info(const cms::string& what) { FW::G::Logger.info(what); }
void warning(const cms::string& what) { FW::G::Logger.warning(what); }
void error(const cms::string& what) { FW::G::Logger.error(what); }
void fatal(const cms::string& what) { FW::G::Logger.fatal(what); }

PYBIND11_EMBEDDED_MODULE(cmsext_log, m) {
  m.def(
      "debug",
      [](const std::string& message, const std::string& function = "",
         const std::string& moduleGlobalName = "") {
        FW::G::Logger.debug(message, function, moduleGlobalName);
      },
      pybind11::arg("message"), pybind11::arg("function") = "",
      pybind11::arg("moduleGlobalName") = ""),
      m.def(
          "info",
          [](const std::string& message, const std::string& function = "",
             const std::string& moduleGlobalName = "") {
            FW::G::Logger.info(message, function, moduleGlobalName);
          },
          pybind11::arg("message"), pybind11::arg("function") = "",
          pybind11::arg("moduleGlobalName") = ""),
      m.def(
          "warning",
          [](const std::string& message, const std::string& function = "",
             const std::string& moduleGlobalName = "") {
            FW::G::Logger.warning(message, function, moduleGlobalName);
          },
          pybind11::arg("message"), pybind11::arg("function") = "",
          pybind11::arg("moduleGlobalName") = ""),
      m.def(
          "error",
          [](const std::string& message, const std::string& function = "",
             const std::string& moduleGlobalName = "") {
            FW::G::Logger.info(message, function, moduleGlobalName);
          },
          pybind11::arg("message"), pybind11::arg("function") = "",
          pybind11::arg("moduleGlobalName") = ""),

      m.def(
          "fatal",
          [](const std::string& message, const std::string& function = "",
             const std::string& moduleGlobalName = "") {
            FW::G::Logger.debug(message, function, moduleGlobalName);
          },
          pybind11::arg("message"), pybind11::arg("function") = "",
          pybind11::arg("moduleGlobalName") = "");
}
}  // namespace cmsext::log