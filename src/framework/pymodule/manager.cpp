#include "manager.h"

#include "../core/application.h"
#include "../core/logger.h"
#include "../global.h"
#include "../util/file.h"
#include "callback.h"
#include "module.h"
#include "signalmanager.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <map>

#include <pybind11/embed.h>
#include <pybind11/eval.h>
#include <pybind11/stl.h>

constexpr auto INIT_FILE = "__init__.py";

namespace FW::PyModule {
void Manager::init() {
  if (FW_DEF_PATH_MODULES == "modules") {
    modules_path_.append(G::Application->get_working_directory());
    modules_path_.append(FW_DEF_PATH_MODULES);
  } else {
    modules_path_ = std::filesystem::path(FW_DEF_PATH_MODULES);
  }

  std::wstring pyPath;
#ifdef FW_DEF_PATH_PYTHON_LIB
  std::filesystem::path pythonLibPath(FW_DEF_PATH_PYTHON_LIB);
  pyPath = pythonLibPath.wstring();
#else   // #ifdef FW_DEF_PATH_PYTHON_LIB
  pyPath = Py_GetPath();
#endif  // #ifdef FW_DEF_PATH_PYTHON_LIB

#if defined(FW_PLATFORM_WINDOWS)
  Py_SetPath((pyPath + L";" + modules_path_.wstring()).c_str());
  Py_SetPythonHome((modules_path_.wstring()).c_str());
#elif defined(FW_PLATFORM_LINUX)
  Py_SetPath((pyPath + L":" + modules_path_.wstring()).c_str());
#endif  // #if defined(FW_PLATFORM_WINDOWS)

  Py_OptimizeFlag = 1;
  Py_SetProgramName(L"Modular Scripting");

  init_modules();
}

void Manager::terminate() { terminate_modules(); }

bool Manager::load_and_import_module(const std::string& name) {
  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug("Load and import module[" + name + "]");)
  std::filesystem::path path(modules_path_);
  path.append(name);
  if (!std::filesystem::exists(path)) {
    path.replace_extension(".py");
    if (!std::filesystem::exists(path)) {
      path.replace_extension("");
      FW::G::Logger.error("Given module [" + name + "] does not exist.");
      return false;
    }
  }

  auto module = load_module(path);

  if (module == nullptr) return false;
  if (!import_module(module)) {
    remove_module(module);
    return false;
  }
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("Done # Load and import");)
  return true;
}

void Manager::restart_modules() {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("Restart modules... ");)
  terminate_modules();
  init_modules();
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("[DONE]");)
}

void Manager::reload_module(const std::string& global_name) {
  std::string script = R"(
import TO_REPLACE
import importlib
print("Reload module: [TO_REPLACE].")
importlib.reload(TO_REPLACE)
    )";
  boost::replace_all(script, "TO_REPLACE", global_name);
  pybind11::exec(script);

  // Connected to on_reload can be only top level module, so we send signal to
  // top level module with information of reloaded sub module.
  std::vector<std::string> splittedModuleGlobalName;
  boost::algorithm::split(splittedModuleGlobalName, global_name,
                          boost::algorithm::is_any_of("."));
  // INDEX[0] is top level module name
  if (splittedModuleGlobalName.empty() ||
      !module_map_.count(splittedModuleGlobalName[0])) {
    FW::G::Logger.error("Cant send reload signal to top level module [" +
                        splittedModuleGlobalName[0] + "] is not mapped.");
    return;
  }
  module_map_[splittedModuleGlobalName[0]]->get_object().attr(Signal::on_reload)(
      global_name);
}

void Manager::connect(const std::string& signal,
                      pybind11::object function_object,
                      pybind11::object* global_variable_object) {
  connect(signal, create_call_back(function_object, global_variable_object));
}

void Manager::connect(const std::string& signal, Callback_ptr callback) {
  FW::G::PyModule_Manager->signal_manager.connect(signal, callback);
}

void Manager::disconnect(const std::string& signal,
                         pybind11::object function_object,
                         pybind11::object* global_variable_object) {
  FW::G::PyModule_Manager->signal_manager.disconnect(
      signal, create_call_back(function_object, global_variable_object));
}

void Manager::run_script(const std::string& script) { pybind11::exec(script); }

Module_ptr Manager::load_module(const std::filesystem::path& path) {
  auto path_without_extension = path.u8string();
  auto file_name_without_extension = path.filename().u8string();
  bool is_package = false;

  try {
    if (std::filesystem::is_directory(path)) {
      if (!std::filesystem::exists(path_without_extension + "/" + INIT_FILE)) {
        FW::G::Logger.error("Unable to load package: [" +
                            file_name_without_extension +
                            "] does not constains [" + INIT_FILE + "].");
        return nullptr;
      }
      is_package = true;
    } else if (std::filesystem::is_regular_file(path)) {
      if (path.extension().u8string() != ".py")
        return nullptr;  // Not module file
      std::filesystem::path filename = path.filename();
      file_name_without_extension = filename.replace_extension("").u8string();
    } else {
      return nullptr;  // Unknown file
    }

    const auto& module_name = file_name_without_extension;

    if (module_map_.count(module_name)) {
      FW::G::Logger.error("Unable to add module[" + module_name +
                          "]. Already exist.");
      return nullptr;
    }

    FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug(
        "  Found module name: [" + module_name + "]. Full name: [" +
        path_without_extension + "]. Package: [" +
        (is_package ? "true" : "false") + "].");)
    module_map_[module_name] =
        std::make_shared<Module>(module_name, path_without_extension, is_package);
  } catch (std::exception& e) {
    FW::G::Logger.error("Unable to add module [" + file_name_without_extension +
                        "]. Exception :\n\t " + std::string(e.what()));
  } catch (...) {
    FW::G::Logger.error("Unable to add module [" + file_name_without_extension +
                        "]. Exception: \n\tUnknown error.");
    return nullptr;
  }
  return module_map_[file_name_without_extension];
}

void Manager::load_modules() {
  pybind11::exec(R"(
            import sys
            sys.dont_write_bytecode = True
        )");

  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug(
      ">>>> Check if directory [" + modules_path_.string() + "] exist.");)

  if (!std::filesystem::exists(modules_path_)) {
    FW::G::Logger.error("Path [" + modules_path_.string() + "] does NOT exist.");
    return;
  }

  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug(">>>> Discover files in given directory [" +
                          modules_path_.string() + "]");)

  for (const auto& entry : std::filesystem::directory_iterator(modules_path_))
    load_module(entry.path());

  auto module_map_it = module_map_.begin();
  while (module_map_it != module_map_.end()) {
    if (import_module((*module_map_it).second)) {
      module_map_it++;
    } else {
      auto module = (*module_map_it).second;
      module_map_it = module_map_.erase(module_map_it);
      remove_module(module);
    }
  }
}

bool Manager::import_module(Module_ptr& module) {
  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug(">>>> Import module: [" + module->get_name() + "].");)

  try {
    module->set_object(pybind11::module::import(module->get_name().c_str()));
    // These signals can be only connected to top level modules
    {
      auto splitted_function_global_name =
          std::make_shared<std::vector<std::string>>();
      splitted_function_global_name->push_back(module->get_name());
      splitted_function_global_name->push_back(std::string(Signal::on_load));

      connect(std::string(Signal::on_load),
              std::make_shared<Callback>(module, splitted_function_global_name));
    }
    {
      auto splitted_function_global_name =
          std::make_shared<std::vector<std::string>>();
      splitted_function_global_name->push_back(module->get_name());
      splitted_function_global_name->push_back(std::string(Signal::on_reload));

      connect(std::string(Signal::on_reload),
              std::make_shared<Callback>(module, splitted_function_global_name));
    }
  } catch (std::exception& e) {
    FW::G::Logger.error("Unable to import module, error : " +
                        std::string(e.what()));
    return false;
  }
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("Done # Import module");)
  return true;
}

void Manager::remove_module(Module_ptr& module) {
  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug(">>>> Remove module: [" + module->get_name() + "].");)

  module_map_.erase(module->get_name());
  signal_manager.disconnect(module);

  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("Done # Remove module");)
}

Callback_ptr Manager::create_call_back(
    pybind11::object function_object,
    pybind11::object* global_variable_object) const {
  std::string module_global_name =
      function_object.attr("__module__").cast<std::string>();

  std::shared_ptr<std::vector<std::string>> splitted_function_global_name =
      std::make_shared<std::vector<std::string>>();
  boost::algorithm::split((*splitted_function_global_name), module_global_name,
                          boost::algorithm::is_any_of("."));

  std::string global_variable;
  if (global_variable_object != nullptr && !global_variable_object->is_none()) {
    auto globals = pybind11::globals();

    for (auto [name, object] : globals) {
      if (name.is(*global_variable_object)) global_variable = pybind11::str(name);
    }
  }
  bool call_from_global_variable = !global_variable.empty();

  if (call_from_global_variable)
    splitted_function_global_name->push_back(global_variable);

  std::string function = function_object.attr("__name__").cast<std::string>();
  splitted_function_global_name->push_back(function);

  auto& top_level_module_name = (*splitted_function_global_name)[0];
  auto module_it = module_map_.find(top_level_module_name);
  if (module_it == module_map_.end()) {
    FW::G::Logger.error("Unable to create callback. Function[" + function +
                        "]. Global variable [" + global_variable +
                        "]. Module [" + top_level_module_name +
                        "] is not mapped.");
    return nullptr;
  }

  auto& module = module_it->second;
  return std::make_shared<Callback>(module, splitted_function_global_name,
                                    call_from_global_variable);
}
void Manager::init_modules() {
  if (Py_IsInitialized()) {
    FW::G::Logger.error(
        "Cant initialize modules. You need to terminate modules before new "
        "initialize.");
    return;
  }
  pybind11::initialize_interpreter();
  load_modules();
  signal_manager.send(Signal::on_load);
}

void Manager::terminate_modules() {
  if (!Py_IsInitialized()) return;
  signal_manager.terminate();
  module_map_.clear();
  pybind11::finalize_interpreter();
}
}  // namespace FW::PyModule
