#ifndef FW_PYMODULE_MODULE_H
#define FW_PYMODULE_MODULE_H

#define HAVE_SNPRINTF 1
#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "declarations.h"

namespace FW::PyModule {
struct Module {
  Module(const std::string& fileNameWithoutExtension,
         const std::string& pathWithoutExtension, bool isPackage = false);

  std::string getName() const;
  std::string getPathWithoutExtension() const;
  pybind11::module getObject() const;

  void setObject(const pybind11::module& newObject);

 protected:
  std::string fileNameWithoutExtension_;
  // Contains modules path
  std::string pathWithoutExtension_;
  bool isPackage_;
  pybind11::module object_;
};
}  // namespace FW::PyModule
#endif  // #ifndef FW_PYMODULE_MODULE_H
