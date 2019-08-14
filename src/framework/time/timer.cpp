#include "timer.h"

namespace FW::Time {
Timer::Timer() : running_{false} {}

void Timer::start() {
  if (running_) return;
  running_ = true;
  start_time_.reset_to_current_time();
}

void Timer::stop() {
  if (!running_) return;
  remaining_time_.reset_to_micros(remaining_time_in_micros());
  running_ = false;
}

void Timer::reset() {
  start_time_.set_null();
  remaining_time_ = set_time_;
}

bool Timer::is_running() { return running_; }

void Timer::set_countdown_in_micros(ticks_t micros) {
  set_time_.reset_to_micros(micros);
  remaining_time_ = set_time_;
}

void Timer::set_countdown_in_millis(ticks_t millis) {
  set_time_.reset_to_millis(millis);
  remaining_time_ = set_time_;
}

void Timer::set_countdown_in_seconds(ticks_t seconds) {
  set_time_.reset_to_second(seconds);
  remaining_time_ = set_time_;
}

ticks_t Timer::remaining_time_in_micros() {
  if (!running_) return -1;
  ticks_t timeDiff = Now::get_micros() - set_time_.to_micros();
  if (timeDiff > remaining_time_.to_micros()) return 0;
  return timeDiff;
}

ticks_t Timer::remaining_time_in_millis() {
  if (!running_) return -1;
  ticks_t timeDiff = Now::get_millis() - set_time_.to_millis();
  if (timeDiff > remaining_time_.to_millis()) return 0;
  return timeDiff;
}

ticks_t Timer::remaining_time_in_seconds() {
  if (!running_) return -1;
  ticks_t timeDiff = Now::get_seconds() - set_time_.to_seconds();
  if (timeDiff > remaining_time_.to_seconds()) return 0;
  return timeDiff;
}

}  // namespace FW::Time