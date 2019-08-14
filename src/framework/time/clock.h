#ifndef FW_TIME_CLOCK_H
#define FW_TIME_CLOCK_H

#include "declarations.h"

#include "time.h"

namespace FW::Time {
class Clock {
 public:
  void update();

  ticks_t to_micros();
  ticks_t to_millis();
  ticks_t to_seconds();

 private:
  Time time_;
};
}  // namespace FW::Time
#endif  // #ifndef FW_TIME_CLOCK_H