#include "event.h"

namespace FW::Thread {
Event::Event() {}

void Event::execute(Time::ticks_t syncTime) {}

void Event::cancel() {}

bool Event::canExecuteRepeat(Time::ticks_t syncTime) const { return false; }
}  // namespace FW::Thread