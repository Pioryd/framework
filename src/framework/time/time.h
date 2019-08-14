#ifndef FW_TIME_TIME_H
#define FW_TIME_TIME_H

#include "declarations.h"

namespace FW::Time {
struct Time {
  Time();

  void reset_to_current_time();
  void reset_to_micros(ticks_t micros);
  void reset_to_millis(ticks_t milis);
  void reset_to_second(ticks_t seconds);

  ticks_t to_micros();
  ticks_t to_millis();
  ticks_t to_seconds();

  void set_null();
  bool is_null();

 protected:
  ticks_t micros_;
  ticks_t millis_;
  ticks_t seconds_;
};

namespace Now {
std::string get_time();
ticks_t get_micros();
ticks_t get_millis();
ticks_t get_seconds();
}  // namespace Now
}  // namespace FW::Time
#endif  // #ifndef FW_TIME_TIME_H