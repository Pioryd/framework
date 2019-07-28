#ifndef FW_TIME_TIMER_H
#define FW_TIME_TIMER_H

#include "declarations.h"

namespace FW::Time {
class Timer {
 public:
  Timer();

  void start();
  void stop();
  void reset();

  bool isRunning();

  // Remaining time is reset after set
  void setCountdownInMicros(ticks_t micros);
  // Remaining time is reset after set
  void setCountdownInMillis(ticks_t milis);
  // Remaining time is reset after set
  void setCountdownInSecond(ticks_t seconds);

  // Return -1 if not running. Return 0 if the countdown has been completed.
  ticks_t remainingTimeInMicros();
  // Return -1 if not running. Return 0 if the countdown has been completed
  ticks_t remainingTimeInMillis();
  // Return -1 if not running. Return 0 if the countdown has been completed
  ticks_t remainingTimeInSeconds();

 private:
  Time startTime_;
  Time remainingTime_;
  Time setTime_;
  bool running_;
};
}  // namespace FW::Time
#endif  // #ifndef FW_TIME_TIMER_H