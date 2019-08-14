#ifndef FW_PYMODULE_MODULE_H
#define FW_PYMODULE_MODULE_H

#define HAVE_SNPRINTF 1
#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "declarations.h"

namespace FW::PyModule {
struct Module {
  Module(const std::string& file_name_without_extension,
         const std::string& path_without_extension, bool is_package = false);

  std::string get_name() const;
  std::string get_path_without_extension() const;
  pybind11::module get_object() const;

  void set_object(const pybind11::module& newObject);

 protected:
  std::string file_name_without_extension_;
  // Contains modules path
  std::string path_without_extension_;
  bool is_package_;
  pybind11::module object_;
};
}  // namespace FW::PyModule
#endif  // #ifndef FW_PYMODULE_MODULE_H
