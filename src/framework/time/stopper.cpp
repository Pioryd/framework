#include "stopper.h"

namespace FW::Time {
Stopper::Stopper() : running_{false} {}

void Stopper::start() {}

void Stopper::stop() {}

void Stopper::restart() {}

bool Stopper::isRunning() { return running_; }

ticks_t Stopper::elapsedTimeInMicros() { return 0; }

ticks_t Stopper::elapsedTimeInMillis() { return 0; }

ticks_t Stopper::elapsedTimeInSeconds() { return 0; }
}  // namespace FW::Time