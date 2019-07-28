#include "stopper.h"

namespace FW::Time {
Stopper::Stopper() : running_{false} {}

void Stopper::start() {
  running_ = true;
  startTime_.resetToCurrentTime();
  elapsedTime_.setNull();
}

void Stopper::stop() {
  elapsedTime_.resetToMicros(elapsedTimeInMicros());
  startTime_.setNull();
  running_ = false;
}

void Stopper::restart() {
  stop();
  start();
}

bool Stopper::isRunning() { return running_; }

ticks_t Stopper::elapsedTimeInMicros() {
  if (!running_) return -1;
  return Now::getMicros() - startTime_.toMicros() + elapsedTime_.toMicros();
}

ticks_t Stopper::elapsedTimeInMillis() {
  if (!running_) return -1;
  return Now::getMillis() - startTime_.toMillis() + elapsedTime_.toMillis();
}

ticks_t Stopper::elapsedTimeInSeconds() {
  if (!running_) return -1;
  return Now::getSeconds() - startTime_.toSeconds() + elapsedTime_.toSeconds();
}
}  // namespace FW::Time