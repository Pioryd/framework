#include "module.h"

#include "callback.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

namespace FW::PyModule {
Module::Module(const std::string& fileNameWithoutExtension,
               const std::string& pathWithoutExtension, bool isPackage)
    : fileNameWithoutExtension_(fileNameWithoutExtension),
      pathWithoutExtension_(pathWithoutExtension),
      isPackage_(isPackage) {}

std::string Module::getName() const { return fileNameWithoutExtension_; }

std::string Module::getPathWithoutExtension() const {
  return pathWithoutExtension_;
}

pybind11::module Module::getObject() const { return object_; }

void Module::setObject(const pybind11::module& newObject) {
  object_ = newObject;
}
}  // namespace FW::PyModule
