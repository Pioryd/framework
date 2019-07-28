#ifndef FW_TIME_TIME_H
#define FW_TIME_TIME_H

#include "declarations.h"

namespace FW::Time {
struct Time {
  Time();

  void resetToCurrentTime();
  void resetToMicros(ticks_t micros);
  void resetToMillis(ticks_t milis);
  void resetToSecond(ticks_t seconds);

  ticks_t toMicros();
  ticks_t toMillis();
  ticks_t toSeconds();

  void setNull();
  bool isNull();

 protected:
  ticks_t micros_;
  ticks_t millis_;
  ticks_t seconds_;
};
}  // namespace FW::Time
#endif  // #ifndef FW_TIME_TIME_H