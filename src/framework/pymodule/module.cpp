#include "module.h"

namespace FW::PyModule {
Module::Module(const std::string& fileNameWithoutExtension,
               const std::string& pathWithoutExtension, bool isPackage) {}

std::string Module::getName() const { return ""; }

std::string Module::getPathWithoutExtension() const { return ""; }

pybind11::module Module::getObject() const { return pybind11::module; }

void Module::setObject(const pybind11::module& newObject) {}
}  // namespace FW::PyModule
