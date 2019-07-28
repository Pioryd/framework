#ifndef MODULES_UI_H
#define MODULES_UI_H

#include <framework/pch.h>
#include <framework/pymodule/manager.h>

namespace Modules {
namespace UI {
void init(const std::vector<std::string>& argv, const std::string& title);
void createUI();
}  // namespace UI
}  // namespace Modules
#endif MODULES_UI_H