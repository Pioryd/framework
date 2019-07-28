#include "callback.h"

namespace FW::PyModule {
Callback::Callback(
    Module_ptr topLevelModule,
    std::shared_ptr<std::vector<std::string>> splittedFunctionGlobalName,
    bool callFromGlobalVariable) {}

bool Callback::operator==(const Callback& object) const { return false; }

bool Callback::operator!=(const Callback& object) const { return false; }

Module_ptr Callback::getTopLevelModule() const { return Module_ptr; }

std::string Callback::getModuleTopLevelName() const { return ""; }

std::string Callback::getFunctionName() const { return ""; }

std::string Callback::getFunctionGlobalName() const { return ""; }

std::string Callback::getGlobalVariableName() const { return ""; }

bool Callback::isCallFromGlobalVariable() const { return false; }
}  // namespace FW::PyModule
