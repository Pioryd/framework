#include "manager.h"

namespace FW::PyModule {
void Manager::init() {}

void Manager::terminate() {}

bool Manager::loadAndImportModule(const std::string& name) { return false; }

void Manager::restartModules() {}

void Manager::reloadModule(const std::string& globalName) {}

void Manager::connect(const std::string& signal,
                      pybind11::object functionObject,
                      pybind11::object* globalVariableObject) {}

void Manager::connect(const std::string& signal, Callback_ptr callback) {}

void Manager::disconnect(const std::string& signal,
                         pybind11::object functionObject,
                         pybind11::object* globalVariableObject) {}

Module_ptr Manager::loadModule(const std::filesystem::path& path) {
  return Module_ptr();
}

void Manager::loadModules() {}

bool Manager::importModule(Module_ptr& module) { return false; }

void Manager::removeModule(Module_ptr& module) {}

Callback_ptr Manager::createCallBack(
    pybind11::object functionObject,
    pybind11::object* globalVariableObject) const {
  return Callback_ptr;
}

void Manager::initModules() {}

void Manager::terminateModules() {}
}  // namespace FW::PyModule
