#include "callback.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

namespace FW::PyModule {
Callback::Callback(
    Module_ptr top_level_module,
    std::shared_ptr<std::vector<std::string>> splitted_function_global_name,
    bool call_from_global_variable)
    : top_level_module_{top_level_module},
      splitted_function_global_name_{splitted_function_global_name},
      call_from_global_variable_{call_from_global_variable} {}

bool Callback::operator==(const Callback& object) const {
  return (top_level_module_.get() == object.top_level_module_.get() &&
          *splitted_function_global_name_ == *object.splitted_function_global_name_);
}
bool Callback::operator!=(const Callback& object) const {
  return (top_level_module_.get() != object.top_level_module_.get() ||
          *splitted_function_global_name_ != *object.splitted_function_global_name_);
}

Module_ptr Callback::get_top_level_module() const { return top_level_module_; }

std::string Callback::get_module_top_level_name() const {
  if (splitted_function_global_name_->empty()) return "";
  return (*splitted_function_global_name_)[0];
}

std::string Callback::get_function_name() const {
  if ((call_from_global_variable_ && splitted_function_global_name_->size() < 3) ||
      (!call_from_global_variable_ && splitted_function_global_name_->size() < 2))
    return "";
  return (
      *splitted_function_global_name_)[splitted_function_global_name_->size() - 1];
}

std::string Callback::get_function_global_name() const {
  std::string function_global_name;
  for (uint32_t i = 0; i < splitted_function_global_name_->size(); i++) {
    function_global_name.append((*splitted_function_global_name_)[i]);
    if (i < splitted_function_global_name_->size() - 1)
      function_global_name.append(".");
  }
  return function_global_name;
}
std::string Callback::get_global_variable_name() const {
  if (splitted_function_global_name_->size() < 3) return "";
  if (!call_from_global_variable_) return "";
  return (
      *splitted_function_global_name_)[splitted_function_global_name_->size() - 2];
}

bool Callback::is_call_from_global_variable() const {
  return call_from_global_variable_;
}
}  // namespace FW::PyModule
