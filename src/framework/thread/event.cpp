#include "event.h"

namespace FW::Thread {
Event::Event()
    : guid{Event::FATAL_GUID},
      callback(nullptr),
      executeTime{NO_DELAY},
      expirationTime{DO_NOT_EXPIRE},
      waitTimeBetweenRepeats{MINIMUM_WAIT_TIME},
      maxExecuteRepeats{REPEAT_ONCE},
      active{true},
      executeCount{0} {}

void Event::execute(Time::ticks_t syncTime) {
  if (active && callback && canExecuteRepeat(syncTime)) {
    callback();

    // prepare next repeat
    executeCount++;

    if (canExecuteRepeat(syncTime)) {
      executeTime += waitTimeBetweenRepeats;
      return;
    }
  }
  cancel();
}

void Event::cancel() {
  active = false;
  callback = nullptr;
}

bool Event::canExecuteRepeat(Time::ticks_t syncTime) const {
  // The order of checking is important
  if (active == false) return false;

  if ((expirationTime != DO_NOT_EXPIRE) && (expirationTime < syncTime))
    return false;

  if (maxExecuteRepeats == REPEAT_ALL_THE_TIME)
    return true;
  else if (executeCount < maxExecuteRepeats)
    return true;

  return false;
}
}  // namespace FW::Thread
