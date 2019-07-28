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

  EventPoll(uint32_t id, const std::string& traceInfo);
  virtual ~EventPoll() = default;

  EventPoll(const EventPoll&) = delete;
  EventPoll& operator=(const EventPoll&) = delete;

  // Returns GUID of set event. If fail returns Event::FATAL_GUID.
  EventGUID addInstantEvent(std::function<void(void)> callback,
                            bool pushFront = false);
  EventGUID addDelayedEvent(
      std::function<void(void)> callback,
      Time::ticks_t executeDelay = Event::NO_DELAY,
      int32_t maxExecuteRepeats = Event::REPEAT_ONCE,
      Time::ticks_t waitTimeBetweenRepeats = Event::MINIMUM_WAIT_TIME,
      Time::ticks_t expiration = Event::DO_NOT_EXPIRE);
  // Return true when remove event. False when event with given GUID does not
  // exist.
  bool removeEvent(EventGUID eventId);
  State getState();

  void shutdown();
  void poll();

 protected:
  inline bool isExecutedEventsLimitExceeded(uint32_t executedEventsCount);
  inline Event_ptr createEvent(
      std::function<void(void)> callback,
      Time::ticks_t executeDelay = Event::NO_DELAY,
      int32_t maxExecuteRepeats = Event::REPEAT_ONCE,
      Time::ticks_t waitTimeBetweenRepeats = Event::MINIMUM_WAIT_TIME,
      Time::ticks_t expiration = Event::DO_NOT_EXPIRE);

 public:
  Config config;

 protected:
  const uint32_t id;
  State state;
  EventGUID lastSetEventGUID;
  std::unordered_set<uint32_t> takenEventIds;

  // These events are execute as fast as possible
  std::list<Event_ptr> instantEventList;
  // Use only for cancel Events, becouse std::priority_queue does not have
  // iterator
  std::list<Event_ptr> delayedEventList;
  // Need for fast set events ordered by execute time. Even if event is
  // canceled, will be removed only when the time comes to execute that event.
  std::priority_queue<Event_ptr, std::deque<Event_ptr>, Event::Compare>
      delayedEventQueue;
  // Status of all class functionality
  bool enabled;
  Time::ticks_t pollStartTime;

 private:
  friend EventManager;
};
}  // namespace FW::Thread
#endif  // FW_THREAD_EVENTPOLL_H
