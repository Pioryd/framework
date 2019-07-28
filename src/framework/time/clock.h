#ifndef FW_TIME_CLOCK_H
#define FW_TIME_CLOCK_H

#include "declarations.h"

#include "time.h"

namespace FW::Time {
class Clock {
 public:
  void update();

  ticks_t toMicros();
  ticks_t toMillis();
  ticks_t toSeconds();

 private:
  Time time_;
};
}  // namespace FW::Time
#endif  // #ifndef FW_TIME_CLOCK_H