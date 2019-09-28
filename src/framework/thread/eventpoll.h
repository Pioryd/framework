#ifndef FW_THREAD_EVENTPOLL_H
#define FW_THREAD_EVENTPOLL_H

#include "../core/traceinfo.h"
#include "event.h"

#include <queue>
#include <unordered_set>

namespace FW::Thread {
class EventPoll : public Core::TraceInfo {
 public:
  struct Config {
    bool debug_enabled = true;
  };

 public:
  enum class State { NO_EVENTS, READY_TO_EXECUTE, DELAYED_EVENTS };

  EventPoll(uint32_t id, const std::string& trace_info);
  virtual ~EventPoll() = default;

  EventPoll(const EventPoll&) = delete;
  EventPoll& operator=(const EventPoll&) = delete;

  // Returns GUID of set event. If fail returns Event::FATAL_GUID.
  EventGUID add_instant_event(std::function<void(void)> callback,
                            bool push_front = false);
  EventGUID add_delayed_event(
      std::function<void(void)> callback,
      Time::ticks_t execute_delay = Event::NO_DELAY,
      int32_t max_execute_repeats = Event::REPEAT_ONCE,
      Time::ticks_t wait_time_between_repeats = Event::MINIMUM_WAIT_TIME,
      Time::ticks_t expiration = Event::DO_NOT_EXPIRE);
  // Return true when remove event. False when event with given GUID does not
  // exist.
  bool remove_event(EventGUID event_guid);
  State get_state();

  void shutdown();
  void poll();

 protected:
  inline bool is_executed_events_limit_exceeded(uint32_t executed_events_count);
  inline Event_ptr create_event(
      std::function<void(void)> callback,
      Time::ticks_t execute_delay = Event::NO_DELAY,
      int32_t max_execute_repeats = Event::REPEAT_ONCE,
      Time::ticks_t wait_time_between_repeats = Event::MINIMUM_WAIT_TIME,
      Time::ticks_t expiration = Event::DO_NOT_EXPIRE);

 public:
  Config config;

 protected:
  const uint32_t id_;
  State state_;
  EventGUID last_set_event_guid_;
  std::unordered_set<uint32_t> taken_event_ids_;

  // These events are execute as fast as possible
  std::list<Event_ptr> instant_event_list_;
  // Use only for cancel Events, becouse std::priority_queue does not have
  // iterator
  std::list<Event_ptr> delayed_event_list_;
  // Need for fast set events ordered by execute time. Even if event is
  // canceled, will be removed only when the time comes to execute that event.
  std::priority_queue<Event_ptr, std::deque<Event_ptr>, Event::Compare>
      delayed_event_queue_;
  // Status of all class functionality
  bool enabled_;
  Time::ticks_t poll_start_time_;

 private:
  friend EventManager;
};
}  // namespace FW::Thread
#endif  // FW_THREAD_EVENTPOLL_H
