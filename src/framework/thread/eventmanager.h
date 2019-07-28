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
  EventGUID addSyncEvent(
      std::function<void(void)> callback,
      Time::ticks_t executeDelay = Event::NO_DELAY,
      int32_t maxExecuteRepeats = Event::REPEAT_ONCE,
      Time::ticks_t waitTimeBetweenRepeats = Event::MINIMUM_WAIT_TIME,
      Time::ticks_t expiration = Event::DO_NOT_EXPIRE, bool pushFront = false);
  template <class T>
  EventGUID addSyncEvent(
      std::shared_future<typename std::result_of<T()>::type>& future,
      const T& callback, Time::ticks_t executeDelay = Event::NO_DELAY,
      int32_t maxExecuteRepeats = Event::REPEAT_ONCE,
      Time::ticks_t waitTimeBetweenRepeats = Event::MINIMUM_WAIT_TIME,
      Time::ticks_t expiration = Event::DO_NOT_EXPIRE, bool pushFront = false);
  EventGUID addAsyncEvent(
      std::function<void(void)> callback,
      Time::ticks_t executeDelay = Event::NO_DELAY,
      int32_t maxExecuteRepeats = Event::REPEAT_ONCE,
      Time::ticks_t waitTimeBetweenRepeats = Event::MINIMUM_WAIT_TIME,
      Time::ticks_t expiration = Event::DO_NOT_EXPIRE, bool pushFront = false);
  template <class T>
  EventGUID addAsyncEvent(
      std::shared_future<typename std::result_of<T()>::type>& future,
      const T& callback, Time::ticks_t executeDelay = Event::NO_DELAY,
      int32_t maxExecuteRepeats = Event::REPEAT_ONCE,
      Time::ticks_t waitTimeBetweenRepeats = Event::MINIMUM_WAIT_TIME,
      Time::ticks_t expiration = Event::DO_NOT_EXPIRE, bool pushFront = false);
  bool removeEvent(EventGUID eventGUID);

  void syncPoll();

  void start();
  void join();
  void terminate();

 protected:
  void asyncPoll();

 protected:
  EventPoll syncEventPoll;

  EventPoll asyncEventPoll;
  std::thread asyncThread;
  std::mutex asyncMutex;
  std::atomic<State> asyncState;
  std::condition_variable asyncCondition;
};

template <class T>
EventGUID EventManager::addSyncEvent(
    std::shared_future<typename std::result_of<T()>::type>& future,
    const T& callback, Time::ticks_t executeDelay, int32_t maxExecuteRepeats,
    Time::ticks_t waitTimeBetweenRepeats, Time::ticks_t expiration,
    bool pushFront) {
  auto promise =
      std::make_shared<std::promise<typename std::result_of<T()>::type>>();
  auto eventGUID = addSyncEvent([=]() { promise->set_value(callback()); },
                                executeDelay, maxExecuteRepeats,
                                waitTimeBetweenRepeats, expiration, pushFront);
  future = promise->get_future();
  return eventGUID;
}

template <class T>
EventGUID EventManager::addAsyncEvent(
    std::shared_future<typename std::result_of<T()>::type>& future,
    const T& callback, Time::ticks_t executeDelay, int32_t maxExecuteRepeats,
    Time::ticks_t waitTimeBetweenRepeats, Time::ticks_t expiration,
    bool pushFront) {
  auto promise =
      std::make_shared<std::promise<typename std::result_of<T()>::type>>();
  auto eventGUID = addAsyncEvent([=]() { promise->set_value(callback()); },
                                 executeDelay, maxExecuteRepeats,
                                 waitTimeBetweenRepeats, expiration, pushFront);
  future = promise->get_future();
  return eventGUID;
}
}  // namespace FW::Thread
#endif  // #ifndef FRAMEWORK_THREAD_EVENTMANAGER_H
