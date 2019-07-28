#include "clock.h"

namespace FW::Time {
void Clock::update() { time_.resetToCurrentTime(); }

ticks_t Clock::toMicros() { return time_.toMicros(); }

ticks_t Clock::toMillis() { return time_.toMillis(); }

ticks_t Clock::toSeconds() { return time_.toSeconds(); }
}  // namespace FW::Time