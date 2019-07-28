#include "global.h"

#include "core/logger.h"

namespace FW::G {
FW::Core::Logger Logger;
FW::Thread::LookAfter<FW::Core::Application> Application;
}  // namespace FW::G
