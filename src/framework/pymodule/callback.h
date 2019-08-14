#ifndef FW_PYMODULE_CALLBACK_H
#define FW_PYMODULE_CALLBACK_H

#include "declarations.h"

#include "../global.h"
#include "../core/logger.h"
#include "module.h"

namespace FW::PyModule {
// Callback can be from any module like: package, subpackage, module, submodule.
struct Callback {
  Callback(Module_ptr top_level_module,
           std::shared_ptr<std::vector<std::string>> splitted_function_global_name,
           bool call_from_global_variable = false);

  bool operator==(const Callback& object) const;
  bool operator!=(const Callback& object) const;

  template <typename... Args>
  void call(const Args&... args) const;

  Module_ptr get_top_level_module() const;
  // Like "module"
  std::string get_module_top_level_name() const;
  // Like "function"
  std::string get_function_name() const;
  // Like "module.subModule1...subModule5.optionalVariableName.function"
  std::string get_function_global_name() const;
  // Like "variableName"
  std::string get_global_variable_name() const;
  bool is_call_from_global_variable() const;

 protected:
  Module_ptr top_level_module_;
  std::shared_ptr<std::vector<std::string>> splitted_function_global_name_;
  bool call_from_global_variable_;

 private:
  friend struct Callback;
};

template <typename... Args>
void Callback::call(const Args&... args) const {
  try {
    if (splitted_function_global_name_->size() > 1 && top_level_module_ != nullptr &&
        get_module_top_level_name() == top_level_module_->get_name()) {
      pybind11::object call_function = top_level_module_->get_object();

      // Skip INDEX[0] becouse its name of the top level module object that we
      // get in previous step.
      for (uint32_t i = 1; i < splitted_function_global_name_->size(); i++)
        call_function =
            call_function.attr((*splitted_function_global_name_)[i].c_str());

      call_function(args...);
      return;
    }
  } catch (std::exception& e) {
    FW::G::Logger.error("Exception:\n\t" + std::string(e.what()));
  } catch (...) {
    FW::G::Logger.error("Exception: unknow error.");
  }
  FW::G::Logger.error("Unable to call functions [" + get_function_global_name() +
                      "]");
}
}  // namespace FW::PyModule
#endif  // #ifndef FW_PYMODULE_CALLBACK_H
