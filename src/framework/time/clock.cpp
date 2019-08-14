#include "clock.h"

namespace FW::Time {
void Clock::update() { time_.reset_to_current_time(); }

ticks_t Clock::to_micros() { return time_.to_micros(); }

ticks_t Clock::to_millis() { return time_.to_millis(); }

ticks_t Clock::to_seconds() { return time_.to_seconds(); }
}  // namespace FW::Time