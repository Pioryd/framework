#include "timer.h"

namespace FW::Time {
Timer::Timer() : running_{false} {}

void Timer::start() {}

void Timer::stop() {}

void Timer::reset() {}

bool Timer::isRunning() { return false; }

void Timer::setCountdownInMicros(ticks_t micros) {}

void Timer::setCountdownInMillis(ticks_t millis) {}

void Timer::setCountdownInSecond(ticks_t seconds) {}

ticks_t Timer::remainingTimeInMicros() { return 0; }

ticks_t Timer::remainingTimeInMillis() { return 0; }

ticks_t Timer::remainingTimeInSeconds() { return 0; }

}  // namespace FW::Time