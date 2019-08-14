#ifndef FW_TIME_TIMER_H
#define FW_TIME_TIMER_H

#include "declarations.h"

#include "time.h"

namespace FW::Time {
class Timer {
 public:
  Timer();

  void start();
  void stop();
  void reset();

  bool is_running();

  // Remaining time is reset after set
  void set_countdown_in_micros(ticks_t micros);
  // Remaining time is reset after set
  void set_countdown_in_millis(ticks_t milis);
  // Remaining time is reset after set
  void set_countdown_in_seconds(ticks_t seconds);

  // Return -1 if not running. Return 0 if the countdown has been completed.
  ticks_t remaining_time_in_micros();
  // Return -1 if not running. Return 0 if the countdown has been completed
  ticks_t remaining_time_in_millis();
  // Return -1 if not running. Return 0 if the countdown has been completed
  ticks_t remaining_time_in_seconds();

 private:
  Time start_time_;
  Time remaining_time_;
  Time set_time_;
  bool running_;
};
}  // namespace FW::Time
#endif  // #ifndef FW_TIME_TIMER_H