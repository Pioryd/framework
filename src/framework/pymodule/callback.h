#ifndef FW_PYMODULE_CALLBACK_H
#define FW_PYMODULE_CALLBACK_H

#include "declarations.h"

#include "../global.h"
#include "../core/logger.h"
#include "module.h"

namespace FW::PyModule {
// Callback can be from any module like: package, subpackage, module, submodule.
struct Callback {
  Callback(Module_ptr topLevelModule,
           std::shared_ptr<std::vector<std::string>> splittedFunctionGlobalName,
           bool callFromGlobalVariable = false);

  bool operator==(const Callback& object) const;
  bool operator!=(const Callback& object) const;

  template <typename... Args>
  void call(const Args&... args) const;

  Module_ptr getTopLevelModule() const;
  // Like "module"
  std::string getModuleTopLevelName() const;
  // Like "function"
  std::string getFunctionName() const;
  // Like "module.subModule1...subModule5.optionalVariableName.function"
  std::string getFunctionGlobalName() const;
  // Like "variableName"
  std::string getGlobalVariableName() const;
  bool isCallFromGlobalVariable() const;

 protected:
  Module_ptr topLevelModule_;
  std::shared_ptr<std::vector<std::string>> splittedFunctionGlobalName_;
  bool callFromGlobalVariable_;

 private:
  friend struct Callback;
};

template <typename... Args>
void Callback::call(const Args&... args) const {
  try {
    if (splittedFunctionGlobalName_->size() > 1 && topLevelModule_ != nullptr &&
        getModuleTopLevelName() == topLevelModule_->getName()) {
      pybind11::object callFunction = topLevelModule_->getObject();

      // Skip INDEX[0] becouse its name of the top level module object that we
      // get in previous step.
      for (uint32_t i = 1; i < splittedFunctionGlobalName_->size(); i++)
        callFunction =
            callFunction.attr((*splittedFunctionGlobalName_)[i].c_str());

      callFunction(args...);
      return;
    }
  } catch (std::exception& e) {
    FW::G::Logger.error("Exception:\n\t" + std::string(e.what()));
  } catch (...) {
    FW::G::Logger.error("Exception: unknow error.");
  }
  FW::G::Logger.error("Unable to call functions [" + getFunctionGlobalName() +
                      "]");
}
}  // namespace FW::PyModule
#endif  // #ifndef FW_PYMODULE_CALLBACK_H
