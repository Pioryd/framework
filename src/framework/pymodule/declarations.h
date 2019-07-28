#ifndef FW_PYMODULE_DECLARATIONS_H
#define FW_PYMODULE_DECLARATIONS_H

#include "../pch.h"

namespace FW::PyModule {
struct Module;
struct Callback;
class Manager;
class SignalManager;

typedef std::shared_ptr<Module> Module_ptr;
typedef std::shared_ptr<Callback> Callback_ptr;
typedef std::shared_ptr<Manager> Manager_ptr;
}  // namespace FW::PyModule
#endif  // #ifndef FW_PYMODULE_DECLARATIONS_H
