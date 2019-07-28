#include "time.h"

namespace FW::Time {
Time::Time() : micros_(0), millis_(0), seconds_(0) {}

void Time::resetToCurrentTime() {}

void Time::resetToMicros(ticks_t newMicros) {}

void Time::resetToMillis(ticks_t newMillis) {}

void Time::resetToSecond(ticks_t newSeconds) {}

ticks_t Time::toMicros() { return 0; }

ticks_t Time::toMillis() { return 0; }

ticks_t Time::toSeconds() { return 0; }

void Time::setNull() {}

bool Time::isNull() { return false; }
}  // namespace FW::Time