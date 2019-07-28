#ifndef FW_GLOBAL_H
#define FW_GLOBAL_H

#include "core/declarations.h"
#include "thread/declarations.h"
#include "thread/lookafter.h"

// Globals used by framework and project.
namespace FW::G {
extern FW::Core::Logger Logger;
extern FW::Thread::EventManager eventManager;
extern FW::Thread::LookAfter<FW::Core::Application> Application;
}  // namespace FW::G
#endif  // #ifndef FW_GLOBAL_H
