#include "stopper.h"

namespace FW::Time {
Stopper::Stopper() : running_{false} {}

void Stopper::start() {
  running_ = true;
  start_time_.reset_to_current_time();
  elapsed_time_.set_null();
}

void Stopper::stop() {
  elapsed_time_.reset_to_micros(elapsed_time_in_micros());
  start_time_.set_null();
  running_ = false;
}

void Stopper::restart() {
  stop();
  start();
}

bool Stopper::is_running() { return running_; }

ticks_t Stopper::elapsed_time_in_micros() {
  if (!running_) return -1;
  return Now::get_micros() - start_time_.to_micros() + elapsed_time_.to_micros();
}

ticks_t Stopper::elapsed_time_in_millis() {
  if (!running_) return -1;
  return Now::get_millis() - start_time_.to_millis() + elapsed_time_.to_millis();
}

ticks_t Stopper::elapsed_time_in_seconds() {
  if (!running_) return -1;
  return Now::get_seconds() - start_time_.to_seconds() + elapsed_time_.to_seconds();
}
}  // namespace FW::Time