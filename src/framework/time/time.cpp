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

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace Now {
// For std::chrono::duration_cast<> needs.
const static auto applicationExecuteTime =
    std::chrono::high_resolution_clock::now();

std::string getTime() {
  return to_simple_string(boost::posix_time::second_clock::universal_time());
}

ticks_t getMicros() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::high_resolution_clock::now() - applicationExecuteTime)
      .count();
}

ticks_t getMillis() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::high_resolution_clock::now() - applicationExecuteTime)
      .count();
}

ticks_t getSeconds() {
  return std::chrono::duration_cast<std::chrono::seconds>(
             std::chrono::high_resolution_clock::now() - applicationExecuteTime)
      .count();
}
}  // namespace Now
}  // namespace FW::Time