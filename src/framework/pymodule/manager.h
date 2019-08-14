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
    static constexpr auto on_load = "on_load";
    static constexpr auto on_reload = "on_reload";
  };

 public:
  void init();
  void terminate();

  bool load_and_import_module(const std::string& name);
  void restart_modules();
  // If reload package, the only wil realoaded __init__.py
  void reload_module(const std::string& global_name);

  void connect(const std::string& signal, pybind11::object function_object,
               pybind11::object* global_variable_object = nullptr);
  void connect(const std::string& signal, Callback_ptr callback);

  void disconnect(const std::string& signal, pybind11::object function_object,
                  pybind11::object* global_variable_object = nullptr);

  void run_script(const std::string& script);

 protected:
  Module_ptr load_module(const std::filesystem::path& path);
  void load_modules();
  bool import_module(Module_ptr& module);
  // Can be risky with C++14/17 iter loop, be sure is not inside that loops.
  void remove_module(Module_ptr& module);
  Callback_ptr create_call_back(pybind11::object function_object,
                              pybind11::object* global_variable_object) const;

  void init_modules();
  void terminate_modules();

 public:
  Config config;
  SignalManager signal_manager;

 protected:
  std::filesystem::path modules_path_;
  std::map<std::string, Module_ptr> module_map_;
};
}  // namespace FW::PyModule
#endif  // #ifndef FW_PYMODULE_MANAGER_H
