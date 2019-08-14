#include "module.h"

#include "callback.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

namespace FW::PyModule {
Module::Module(const std::string& file_name_without_extension,
               const std::string& path_without_extension, bool is_package)
    : file_name_without_extension_(file_name_without_extension),
      path_without_extension_(path_without_extension),
      is_package_(is_package) {}

std::string Module::get_name() const { return file_name_without_extension_; }

std::string Module::get_path_without_extension() const {
  return path_without_extension_;
}

pybind11::module Module::get_object() const { return object_; }

void Module::set_object(const pybind11::module& newObject) {
  object_ = newObject;
}
}  // namespace FW::PyModule
