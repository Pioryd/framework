#include "event.h"

namespace FW::Thread {
Event::Event()
    : guid_{Event::FATAL_GUID},
      callback_(nullptr),
      execute_time_{NO_DELAY},
      expiration_time_{DO_NOT_EXPIRE},
      wait_time_between_repeats_{MINIMUM_WAIT_TIME},
      max_execute_repeats_{REPEAT_ONCE},
      active_{true},
      execute_count_{0} {}

void Event::execute(Time::ticks_t sync_time) {
  if (active_ && callback_ && canExecuteRepeat(sync_time)) {
    callback_();

    // prepare next repeat
    execute_count_++;

    if (canExecuteRepeat(sync_time)) {
      execute_time_ += wait_time_between_repeats_;
      return;
    }
  }
  cancel();
}

void Event::cancel() {
  active_ = false;
  callback_ = nullptr;
}

bool Event::canExecuteRepeat(Time::ticks_t sync_time) const {
  // The order of checking is important
  if (active_ == false) return false;

  if ((expiration_time_ != DO_NOT_EXPIRE) && (expiration_time_ < sync_time))
    return false;

  if (max_execute_repeats_ == REPEAT_ALL_THE_TIME)
    return true;
  else if (execute_count_ < max_execute_repeats_)
    return true;

  return false;
}
}  // namespace FW::Thread
