#ifndef FW_THREAD_EVENT_H
#define FW_THREAD_EVENT_H

#include "declarations.h"

#include "../time/declarations.h"

#include <condition_variable>

namespace FW::Thread {
// Its ide thats indetifu event for mall threads
struct EventGUID {
  uint32_t id{0};
  Time::ticks_t time_of_id_set{0};
  uint32_t event_poll_id{0};
  // TODO
  // eventPollId can be 0 or not ?
  bool is_set() {
    if (id != 0 && time_of_id_set != 0) return true;
  }
  void reset() { *this = {}; }
};

class Event {
 public:
  struct Compare {
    bool operator()(const Event_ptr& l_arg, const Event_ptr& r_arg) {
      return l_arg->execute_time_ > r_arg->execute_time_;
    }
  };

 public:
  static constexpr EventGUID FATAL_GUID{0, 0, 0};
  static constexpr Time::ticks_t NO_DELAY{0};
  static constexpr int32_t REPEAT_ONCE{1};
  static constexpr int32_t REPEAT_ALL_THE_TIME{0};
  static constexpr int32_t MINIMUM_WAIT_TIME{10};
  static constexpr Time::ticks_t DO_NOT_EXPIRE{0};

 public:
  Event();
  virtual ~Event() = default;

 public:
  virtual void execute(Time::ticks_t sync_time);
  void cancel();

 protected:
  bool canExecuteRepeat(Time::ticks_t sync_time) const;

 private:  // Const at set
  EventGUID guid_;
  std::function<void()> callback_;
  Time::ticks_t execute_time_;
  Time::ticks_t expiration_time_;
  Time::ticks_t wait_time_between_repeats_;
  int32_t max_execute_repeats_;

 private:  // Work values
  bool active_;
  uint32_t execute_count_;

 private:
  friend class EventPoll;
};

}  // namespace FW::Thread
#endif  // #ifndef FW_THREAD_EVENT_H
