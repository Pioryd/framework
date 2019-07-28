#include "callback.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

namespace FW::PyModule {
Callback::Callback(
    Module_ptr topLevelModule,
    std::shared_ptr<std::vector<std::string>> splittedFunctionGlobalName,
    bool callFromGlobalVariable)
    : topLevelModule_{topLevelModule},
      splittedFunctionGlobalName_{splittedFunctionGlobalName},
      callFromGlobalVariable_{callFromGlobalVariable} {}

bool Callback::operator==(const Callback& object) const {
  return (topLevelModule_.get() == object.topLevelModule_.get() &&
          *splittedFunctionGlobalName_ == *object.splittedFunctionGlobalName_);
}
bool Callback::operator!=(const Callback& object) const {
  return (topLevelModule_.get() != object.topLevelModule_.get() ||
          *splittedFunctionGlobalName_ != *object.splittedFunctionGlobalName_);
}

Module_ptr Callback::getTopLevelModule() const { return topLevelModule_; }

std::string Callback::getModuleTopLevelName() const {
  if (splittedFunctionGlobalName_->empty()) return "";
  return (*splittedFunctionGlobalName_)[0];
}

std::string Callback::getFunctionName() const {
  if ((callFromGlobalVariable_ && splittedFunctionGlobalName_->size() < 3) ||
      (!callFromGlobalVariable_ && splittedFunctionGlobalName_->size() < 2))
    return "";
  return (
      *splittedFunctionGlobalName_)[splittedFunctionGlobalName_->size() - 1];
}

std::string Callback::getFunctionGlobalName() const {
  std::string functionGlobalName;
  for (uint32_t i = 0; i < splittedFunctionGlobalName_->size(); i++) {
    functionGlobalName.append((*splittedFunctionGlobalName_)[i]);
    if (i < splittedFunctionGlobalName_->size() - 1)
      functionGlobalName.append(".");
  }
  return functionGlobalName;
}
std::string Callback::getGlobalVariableName() const {
  if (splittedFunctionGlobalName_->size() < 3) return "";
  if (!callFromGlobalVariable_) return "";
  return (
      *splittedFunctionGlobalName_)[splittedFunctionGlobalName_->size() - 2];
}

bool Callback::isCallFromGlobalVariable() const {
  return callFromGlobalVariable_;
}
}  // namespace FW::PyModule
