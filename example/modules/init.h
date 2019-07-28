#ifndef MODULES_INIT_H
#define MODULES_INIT_H

#include <framework/pch.h>
#include <framework/pymodule/manager.h>

namespace Modules {
void init(const std::vector<std::string>& argv, const std::string& title);
}  // namespace Modules
#endif MODULES_INIT_H