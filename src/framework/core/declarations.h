#ifndef FW_CORE_DECLARATIONS_H
#define FW_CORE_DECLARATIONS_H

#include "../pch.h"

namespace FW::Core {
class Logger;
class Application;
#ifdef FW_DEF_GRAPHICAL_APPLICATION
class ApplicationGraphical;
#else
class ApplicationConsole;
#endif
}  // namespace FW::Core
#endif  // #ifndef FW_CORE_DECLARATIONS_H
