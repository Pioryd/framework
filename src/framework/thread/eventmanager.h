#ifndef FRAMEWORK_THREAD_EVENTMANAGER_H
#define FRAMEWORK_THREAD_EVENTMANAGER_H

#include "eventpoll.h"

#include "../core/traceinfo.h"

#include <atomic>
#include <condition_variable>
#include <future>
#include <thread>

namespace FW::Thread {
class EventManager : public Core::TraceInfo {
  enum class State { RUNNING, WAITING, TERMINATE };

  static constexpr uint32_t SYNC_EVENT_POOL_ID{0};
  static constexpr uint32_t ASYNC_EVENT_POOL_ID{1};
  static constexpr uint32_t AUTO_POLL_ID{0};

 public:
  EventManager(const std::string& traceInfo);
  virtual ~EventManager() = default;

  EventManager(const EventManager&) = delete;
  EventManager& operator=(const EventManager&) = delete;

  // Returns GUID of set event. If fail event.id is Event::FATAL_ID.
  EventGUID add_sync_event(
      std::function<void(void)> callback,
      Time::ticks_t execute_delay = Event::NO_DELAY,
      int32_t max_execute_repeats = Event::REPEAT_ONCE,
      Time::ticks_t wait_time_between_repeats = Event::MINIMUM_WAIT_TIME,
      Time::ticks_t expiration = Event::DO_NOT_EXPIRE, bool push_front = false);
  template <class T>
  EventGUID add_sync_event(
      std::shared_future<typename std::result_of<T()>::type>& future,
      const T& callback, Time::ticks_t execute_delay = Event::NO_DELAY,
      int32_t max_execute_repeats = Event::REPEAT_ONCE,
      Time::ticks_t wait_time_between_repeats = Event::MINIMUM_WAIT_TIME,
      Time::ticks_t expiration = Event::DO_NOT_EXPIRE, bool push_front = false);
  EventGUID add_async_event(
      std::function<void(void)> callback,
      Time::ticks_t execute_delay = Event::NO_DELAY,
      int32_t max_execute_repeats = Event::REPEAT_ONCE,
      Time::ticks_t wait_time_between_repeats = Event::MINIMUM_WAIT_TIME,
      Time::ticks_t expiration = Event::DO_NOT_EXPIRE, bool push_front = false);
  template <class T>
  EventGUID add_async_event(
      std::shared_future<typename std::result_of<T()>::type>& future,
      const T& callback, Time::ticks_t execute_delay = Event::NO_DELAY,
      int32_t max_execute_repeats = Event::REPEAT_ONCE,
      Time::ticks_t wait_time_between_repeats = Event::MINIMUM_WAIT_TIME,
      Time::ticks_t expiration = Event::DO_NOT_EXPIRE, bool push_front = false);
  bool remove_event(EventGUID event_guid);

  void sync_poll();

  void start();
  void join();
  void terminate();

 protected:
  void async_poll();

 protected:
  EventPoll sync_event_poll_;

  EventPoll async_event_poll_;
  std::thread async_thread_;
  std::mutex async_mutex_;
  std::atomic<State> async_state_;
  std::condition_variable asyn_condition_;
};

template <class T>
EventGUID EventManager::add_sync_event(
    std::shared_future<typename std::result_of<T()>::type>& future,
    const T& callback, Time::ticks_t execute_delay, int32_t max_execute_repeats,
    Time::ticks_t wait_time_between_repeats, Time::ticks_t expiration,
    bool push_front) {
  auto promise =
      std::make_shared<std::promise<typename std::result_of<T()>::type>>();
  auto event_guid = add_sync_event([=]() { promise->set_value(callback()); },
                                execute_delay, max_execute_repeats,
                                wait_time_between_repeats, expiration, push_front);
  future = promise->get_future();
  return event_guid;
}

template <class T>
EventGUID EventManager::add_async_event(
    std::shared_future<typename std::result_of<T()>::type>& future,
    const T& callback, Time::ticks_t execute_delay, int32_t max_execute_repeats,
    Time::ticks_t wait_time_between_repeats, Time::ticks_t expiration,
    bool push_front) {
  auto promise =
      std::make_shared<std::promise<typename std::result_of<T()>::type>>();
  auto event_guid = add_async_event([=]() { promise->set_value(callback()); },
                                 execute_delay, max_execute_repeats,
                                 wait_time_between_repeats, expiration, push_front);
  future = promise->get_future();
  return event_guid;
}
}  // namespace FW::Thread
#endif  // #ifndef FRAMEWORK_THREAD_EVENTMANAGER_H
