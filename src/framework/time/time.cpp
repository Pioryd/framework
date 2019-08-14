#include "time.h"

#include <boost/date_time/posix_time/posix_time.hpp>

namespace FW::Time {
Time::Time() : micros_(0), millis_(0), seconds_(0) {}

void Time::reset_to_current_time() {
  micros_ = Now::get_micros();
  millis_ = micros_ / 1000;
  seconds_ = millis_ / 1000;
}

void Time::reset_to_micros(ticks_t newMicros) {
  micros_ = newMicros;
  millis_ = micros_ / 1000;
  seconds_ = millis_ / 1000;
}

void Time::reset_to_millis(ticks_t newMillis) {
  millis_ = newMillis;
  seconds_ = millis_ / 1000;
  micros_ = millis_ * 1000;
}

void Time::reset_to_second(ticks_t newSeconds) {
  seconds_ = newSeconds;
  millis_ = seconds_ * 1000;
  micros_ = millis_ * 1000;
}

ticks_t Time::to_micros() { return micros_; }

ticks_t Time::to_millis() { return millis_; }

ticks_t Time::to_seconds() { return seconds_; }

void Time::set_null() { micros_ = millis_ = seconds_ = 0; }

bool Time::is_null() { return (micros_ == 0 && millis_ == 0 && seconds_ == 0); }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace Now {
// For std::chrono::duration_cast<> needs.
const static auto applicationExecuteTime =
    std::chrono::high_resolution_clock::now();

std::string get_time() {
  return to_simple_string(boost::posix_time::second_clock::universal_time());
}

ticks_t get_micros() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::high_resolution_clock::now() - applicationExecuteTime)
      .count();
}

ticks_t get_millis() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::high_resolution_clock::now() - applicationExecuteTime)
      .count();
}

ticks_t get_seconds() {
  return std::chrono::duration_cast<std::chrono::seconds>(
             std::chrono::high_resolution_clock::now() - applicationExecuteTime)
      .count();
}
}  // namespace Now
}  // namespace FW::Time