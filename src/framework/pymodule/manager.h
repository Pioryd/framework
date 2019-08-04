#ifndef FW_PYMODULE_MANAGER_H
#define FW_PYMODULE_MANAGER_H

#include "declarations.h"

#include "signalmanager.h"

namespace FW::PyModule {
class Manager {
 public:
  struct Config {
    bool debug_enabled = true;
  };

  struct Signal {
    static constexpr auto onLoad = "onLoad";
    static constexpr auto onReload = "onReload";
  };

 public:
  void init();
  void terminate();

  bool loadAndImportModule(const std::string& name);
  void restartModules();
  // If reload package, the only wil realoaded __init__.py
  void reloadModule(const std::string& globalName);

  void connect(const std::string& signal, pybind11::object functionObject,
               pybind11::object* globalVariableObject = nullptr);
  void connect(const std::string& signal, Callback_ptr callback);

  void disconnect(const std::string& signal, pybind11::object functionObject,
                  pybind11::object* globalVariableObject = nullptr);

  void run_script(const std::string& script);

 protected:
  Module_ptr loadModule(const std::filesystem::path& path);
  void loadModules();
  bool importModule(Module_ptr& module);
  // Can be risky with C++14/17 iter loop, be sure is not inside that loops.
  void removeModule(Module_ptr& module);
  Callback_ptr createCallBack(pybind11::object functionObject,
                              pybind11::object* globalVariableObject) const;

  void initModules();
  void terminateModules();

 public:
  Config config;
  SignalManager signalManager;

 protected:
  std::filesystem::path modulesPath_;
  std::map<std::string, Module_ptr> moduleMap_;
};
}  // namespace FW::PyModule
#endif  // #ifndef FW_PYMODULE_MANAGER_H
