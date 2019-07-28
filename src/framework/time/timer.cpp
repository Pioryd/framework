#include "timer.h"

namespace FW::Time {
Timer::Timer() : running_{false} {}

void Timer::start() {
  if (running_) return;
  running_ = true;
  startTime_.resetToCurrentTime();
}

void Timer::stop() {
  if (!running_) return;
  remainingTime_.resetToMicros(remainingTimeInMicros());
  running_ = false;
}

void Timer::reset() {
  startTime_.setNull();
  remainingTime_ = setTime_;
}

bool Timer::isRunning() { return running_; }

void Timer::setCountdownInMicros(ticks_t micros) {
  setTime_.resetToMicros(micros);
  remainingTime_ = setTime_;
}

void Timer::setCountdownInMillis(ticks_t millis) {
  setTime_.resetToMillis(millis);
  remainingTime_ = setTime_;
}

void Timer::setCountdownInSecond(ticks_t seconds) {
  setTime_.resetToSecond(seconds);
  remainingTime_ = setTime_;
}

ticks_t Timer::remainingTimeInMicros() {
  if (!running_) return -1;
  ticks_t timeDiff = Now::getMicros() - setTime_.toMicros();
  if (timeDiff > remainingTime_.toMicros()) return 0;
  return timeDiff;
}

ticks_t Timer::remainingTimeInMillis() {
  if (!running_) return -1;
  ticks_t timeDiff = Now::getMillis() - setTime_.toMillis();
  if (timeDiff > remainingTime_.toMillis()) return 0;
  return timeDiff;
}

ticks_t Timer::remainingTimeInSeconds() {
  if (!running_) return -1;
  ticks_t timeDiff = Now::getSeconds() - setTime_.toSeconds();
  if (timeDiff > remainingTime_.toSeconds()) return 0;
  return timeDiff;
}

}  // namespace FW::Time