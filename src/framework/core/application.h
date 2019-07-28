#ifndef FW_CORE_APPLICATION_H
#define FW_CORE_APPLICATION_H

#include "declarations.h"

#include <framework/core/signal.h>
#include <boost/asio.hpp>

#ifdef FW_DEF_GRAPHICAL_APPLICATION
#include "applicationgraphical.h"
#define APP_PARENT ApplicationGraphical
#else
#include "applicationconsole.h"
#define APP_PARENT ApplicationConsole
#endif

namespace FW::Core {
class Application : public APP_PARENT {};
}  // namespace FW::Core
#endif  // #ifndef FW_CORE_APPLICATION_H
