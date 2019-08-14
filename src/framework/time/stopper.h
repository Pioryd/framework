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

  bool is_running();

  // Return -1 if not running
  ticks_t elapsed_time_in_micros();
  // Return -1 if not running
  ticks_t elapsed_time_in_millis();
  // Return -1 if not running
  ticks_t elapsed_time_in_seconds();

 private:
  Time start_time_;
  Time elapsed_time_;
  Time set_time_;
  bool running_;
};
}  // namespace FW::Time
#endif  // #ifndef FW_TIME_STOPPER_H