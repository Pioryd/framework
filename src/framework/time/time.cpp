#include "time.h"

#include <boost/date_time/posix_time/posix_time.hpp>

namespace FW::Time {
Time::Time() : micros_(0), millis_(0), seconds_(0) {}

void Time::resetToCurrentTime() {
  micros_ = Now::getMicros();
  millis_ = micros_ / 1000;
  seconds_ = millis_ / 1000;
}

void Time::resetToMicros(ticks_t newMicros) {
  micros_ = newMicros;
  millis_ = micros_ / 1000;
  seconds_ = millis_ / 1000;
}

void Time::resetToMillis(ticks_t newMillis) {
  millis_ = newMillis;
  seconds_ = millis_ / 1000;
  micros_ = millis_ * 1000;
}

void Time::resetToSecond(ticks_t newSeconds) {
  seconds_ = newSeconds;
  millis_ = seconds_ * 1000;
  micros_ = millis_ * 1000;
}

ticks_t Time::toMicros() { return micros_; }

ticks_t Time::toMillis() { return millis_; }

ticks_t Time::toSeconds() { return seconds_; }

void Time::setNull() { micros_ = millis_ = seconds_ = 0; }

bool Time::isNull() { return (micros_ == 0 && millis_ == 0 && seconds_ == 0); }
}  // namespace FW::Time