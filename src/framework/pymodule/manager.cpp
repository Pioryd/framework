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
    modulesPath_.append(G::Application->get_working_directory());
    modulesPath_.append(FW_DEF_PATH_MODULES);
  } else {
    modulesPath_ = std::filesystem::path(FW_DEF_PATH_MODULES);
  }

  std::wstring pyPath;
#ifdef FW_DEF_PATH_PYTHON_LIB
  std::filesystem::path pythonLibPath(FW_DEF_PATH_PYTHON_LIB);
  pyPath = pythonLibPath.wstring();
#else   // #ifdef FW_DEF_PATH_PYTHON_LIB
  pyPath = Py_GetPath();
#endif  // #ifdef FW_DEF_PATH_PYTHON_LIB

#if defined(FW_PLATFORM_WINDOWS)
  Py_SetPath((pyPath + L";" + modulesPath_.wstring()).c_str());
  Py_SetPythonHome((modulesPath_.wstring()).c_str());
#elif defined(FW_PLATFORM_LINUX)
  Py_SetPath((pyPath + L":" + modulesPath_.wstring()).c_str());
#endif  // #if defined(FW_PLATFORM_WINDOWS)

  Py_OptimizeFlag = 1;
  Py_SetProgramName(L"Modular Scripting");

  initModules();
}

void Manager::terminate() { terminateModules(); }

bool Manager::loadAndImportModule(const std::string& name) {
  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug("Load and import module[" + name + "]");)
  std::filesystem::path path(modulesPath_);
  path.append(name);
  if (!std::filesystem::exists(path)) {
    path.replace_extension(".py");
    if (!std::filesystem::exists(path)) {
      path.replace_extension("");
      FW::G::Logger.error("Given module [" + name + "] does not exist.");
      return false;
    }
  }

  auto module = loadModule(path);

  if (module == nullptr) return false;
  if (!importModule(module)) {
    removeModule(module);
    return false;
  }
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("Done # Load and import");)
  return true;
}

void Manager::restartModules() {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("Restart modules... ");)
  terminateModules();
  initModules();
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("[DONE]");)
}

void Manager::reloadModule(const std::string& globalName) {
  std::string script = R"(
import TO_REPLACE
import importlib
print("Reload module: [TO_REPLACE].")
importlib.reload(TO_REPLACE)
    )";
  boost::replace_all(script, "TO_REPLACE", globalName);
  pybind11::exec(script);

  // Connected to onReload can be only top level module, so we send signal to
  // top level module with information of reloaded sub module.
  std::vector<std::string> splittedModuleGlobalName;
  boost::algorithm::split(splittedModuleGlobalName, globalName,
                          boost::algorithm::is_any_of("."));
  // INDEX[0] is top level module name
  if (splittedModuleGlobalName.empty() ||
      !moduleMap_.count(splittedModuleGlobalName[0])) {
    FW::G::Logger.error("Cant send reload signal to top level module [" +
                        splittedModuleGlobalName[0] + "] is not mapped.");
    return;
  }
  moduleMap_[splittedModuleGlobalName[0]]->getObject().attr(Signal::onReload)(
      globalName);
}

void Manager::connect(const std::string& signal,
                      pybind11::object functionObject,
                      pybind11::object* globalVariableObject) {
  connect(signal, createCallBack(functionObject, globalVariableObject));
}

void Manager::connect(const std::string& signal, Callback_ptr callback) {
  FW::G::PyModule_Manager->signalManager.connect(signal, callback);
}

void Manager::disconnect(const std::string& signal,
                         pybind11::object functionObject,
                         pybind11::object* globalVariableObject) {
  FW::G::PyModule_Manager->signalManager.disconnect(
      signal, createCallBack(functionObject, globalVariableObject));
}

void Manager::run_script(const std::string& script) { pybind11::exec(script); }

Module_ptr Manager::loadModule(const std::filesystem::path& path) {
  auto pathWithoutExtension = path.u8string();
  auto fileNameWithoutExtension = path.filename().u8string();
  bool isPackage = false;

  try {
    if (std::filesystem::is_directory(path)) {
      if (!std::filesystem::exists(pathWithoutExtension + "/" + INIT_FILE)) {
        FW::G::Logger.error("Unable to load package: [" +
                            fileNameWithoutExtension +
                            "] does not constains [" + INIT_FILE + "].");
        return nullptr;
      }
      isPackage = true;
    } else if (std::filesystem::is_regular_file(path)) {
      if (path.extension().u8string() != ".py")
        return nullptr;  // Not module file
      std::filesystem::path filename = path.filename();
      fileNameWithoutExtension = filename.replace_extension("").u8string();
    } else {
      return nullptr;  // Unknown file
    }

    const auto& moduleName = fileNameWithoutExtension;

    if (moduleMap_.count(moduleName)) {
      FW::G::Logger.error("Unable to add module[" + moduleName +
                          "]. Already exist.");
      return nullptr;
    }

    FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug(
        "  Found module name: [" + moduleName + "]. Full name: [" +
        pathWithoutExtension + "]. Package: [" +
        (isPackage ? "true" : "false") + "].");)
    moduleMap_[moduleName] =
        std::make_shared<Module>(moduleName, pathWithoutExtension, isPackage);
  } catch (std::exception& e) {
    FW::G::Logger.error("Unable to add module [" + fileNameWithoutExtension +
                        "]. Exception :\n\t " + std::string(e.what()));
  } catch (...) {
    FW::G::Logger.error("Unable to add module [" + fileNameWithoutExtension +
                        "]. Exception: \n\tUnknown error.");
    return nullptr;
  }
  return moduleMap_[fileNameWithoutExtension];
}

void Manager::loadModules() {
  pybind11::exec(R"(
            import sys
            sys.dont_write_bytecode = True
        )");

  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug(
      ">>>> Check if directory [" + modulesPath_.string() + "] exist.");)

  if (!std::filesystem::exists(modulesPath_)) {
    FW::G::Logger.error("Path [" + modulesPath_.string() + "] does NOT exist.");
    return;
  }

  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug(">>>> Discover files in given directory [" +
                          modulesPath_.string() + "]");)

  for (const auto& entry : std::filesystem::directory_iterator(modulesPath_))
    loadModule(entry.path());

  auto moduleMap_it = moduleMap_.begin();
  while (moduleMap_it != moduleMap_.end()) {
    if (importModule((*moduleMap_it).second)) {
      moduleMap_it++;
    } else {
      auto module = (*moduleMap_it).second;
      moduleMap_it = moduleMap_.erase(moduleMap_it);
      removeModule(module);
    }
  }
}

bool Manager::importModule(Module_ptr& module) {
  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug(">>>> Import module: [" + module->getName() + "].");)

  try {
    module->setObject(pybind11::module::import(module->getName().c_str()));
    // These signals can be only connected to top level modules
    {
      auto splittedFunctionGlobalName =
          std::make_shared<std::vector<std::string>>();
      splittedFunctionGlobalName->push_back(module->getName());
      splittedFunctionGlobalName->push_back(std::string(Signal::onLoad));

      connect(std::string(Signal::onLoad),
              std::make_shared<Callback>(module, splittedFunctionGlobalName));
    }
    {
      auto splittedFunctionGlobalName =
          std::make_shared<std::vector<std::string>>();
      splittedFunctionGlobalName->push_back(module->getName());
      splittedFunctionGlobalName->push_back(std::string(Signal::onReload));

      connect(std::string(Signal::onReload),
              std::make_shared<Callback>(module, splittedFunctionGlobalName));
    }
  } catch (std::exception& e) {
    FW::G::Logger.error("Unable to import module, error : " +
                        std::string(e.what()));
    return false;
  }
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("Done # Import module");)
  return true;
}

void Manager::removeModule(Module_ptr& module) {
  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug(">>>> Remove module: [" + module->getName() + "].");)

  moduleMap_.erase(module->getName());
  signalManager.disconnect(module);

  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("Done # Remove module");)
}

Callback_ptr Manager::createCallBack(
    pybind11::object functionObject,
    pybind11::object* globalVariableObject) const {
  std::string moduleGlobalName =
      functionObject.attr("__module__").cast<std::string>();

  std::shared_ptr<std::vector<std::string>> splittedFunctionGlobalName =
      std::make_shared<std::vector<std::string>>();
  boost::algorithm::split((*splittedFunctionGlobalName), moduleGlobalName,
                          boost::algorithm::is_any_of("."));

  std::string globalVariable;
  if (globalVariableObject != nullptr && !globalVariableObject->is_none()) {
    auto globals = pybind11::globals();

    for (auto [name, object] : globals) {
      if (name.is(*globalVariableObject)) globalVariable = pybind11::str(name);
    }
  }
  bool callFromGlobalVariable = !globalVariable.empty();

  if (callFromGlobalVariable)
    splittedFunctionGlobalName->push_back(globalVariable);

  std::string function = functionObject.attr("__name__").cast<std::string>();
  splittedFunctionGlobalName->push_back(function);

  auto& topLevelModuleName = (*splittedFunctionGlobalName)[0];
  auto module_it = moduleMap_.find(topLevelModuleName);
  if (module_it == moduleMap_.end()) {
    FW::G::Logger.error("Unable to create callback. Function[" + function +
                        "]. Global variable [" + globalVariable +
                        "]. Module [" + topLevelModuleName +
                        "] is not mapped.");
    return nullptr;
  }

  auto& module = module_it->second;
  return std::make_shared<Callback>(module, splittedFunctionGlobalName,
                                    callFromGlobalVariable);
}
void Manager::initModules() {
  if (Py_IsInitialized()) {
    FW::G::Logger.error(
        "Cant initialize modules. You need to terminate modules before new "
        "initialize.");
    return;
  }
  pybind11::initialize_interpreter();
  loadModules();
  signalManager.send(Signal::onLoad);
}

void Manager::terminateModules() {
  if (!Py_IsInitialized()) return;
  signalManager.terminate();
  moduleMap_.clear();
  pybind11::finalize_interpreter();
}
}  // namespace FW::PyModule
