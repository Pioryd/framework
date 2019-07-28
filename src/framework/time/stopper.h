#ifndef FW_TIME_STOPPER_H
#define FW_TIME_STOPPER_H

#include "declarations.h"

#include "time.h"

namespace FW::Time {
class Stopper {
 public:
  Stopper();

  void start();
  void stop();
  void restart();

  bool isRunning();

  // Return -1 if not running
  ticks_t elapsedTimeInMicros();
  // Return -1 if not running
  ticks_t elapsedTimeInMillis();
  // Return -1 if not running
  ticks_t elapsedTimeInSeconds();

 private:
  Time startTime_;
  Time elapsedTime_;
  Time setTime_;
  bool running_;
};
}  // namespace FW::Time
#endif  // #ifndef FW_TIME_STOPPER_H