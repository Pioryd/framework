#include "eventmanager.h"

#include "../core/logger.h"
#include "../global.h"

namespace FW::Thread {
EventManager::EventManager(const std::string& traceInfo)
    : TraceInfo{traceInfo},
      sync_event_poll_{SYNC_EVENT_POOL_ID, traceInfo + " sync_event_poll_"},
      async_event_poll_{ASYNC_EVENT_POOL_ID, traceInfo + " async_event_poll_"},
      async_state_{State::TERMINATE} {}

EventGUID EventManager::add_sync_event(std::function<void(void)> callback,
                                     Time::ticks_t execute_delay,
                                     int32_t max_execute_repeats,
                                     Time::ticks_t wait_time_between_repeats,
                                     Time::ticks_t expiration, bool push_front) {
  EventGUID return_event_guid = Event::FATAL_GUID;

  if (execute_delay == Event::NO_DELAY) {
    return_event_guid = sync_event_poll_.add_instant_event(callback, push_front);
  } else {
    return_event_guid =
        sync_event_poll_.add_delayed_event(callback, execute_delay, max_execute_repeats,
                                      wait_time_between_repeats, expiration);
  }

  return return_event_guid;
}

EventGUID EventManager::add_async_event(std::function<void(void)> callback,
                                      Time::ticks_t execute_delay,
                                      int32_t max_execute_repeats,
                                      Time::ticks_t wait_time_between_repeats,
                                      Time::ticks_t expiration,
                                      bool push_front) {
  EventGUID return_event_guid = Event::FATAL_GUID;

  async_mutex_.lock();

  if (execute_delay == Event::NO_DELAY) {
    return_event_guid = async_event_poll_.add_instant_event(callback, push_front);
  } else {
    return_event_guid = async_event_poll_.add_delayed_event(
        callback, execute_delay, max_execute_repeats, wait_time_between_repeats,
        expiration);
  }

  if (async_state_ == State::WAITING) {
    async_state_ = State::RUNNING;
    asyn_condition_.notify_one();
  }

  async_mutex_.unlock();

  return return_event_guid;
}

bool EventManager::remove_event(EventGUID eventGUID) {
  bool removed = false;
  if (eventGUID.event_poll_id == SYNC_EVENT_POOL_ID) {
    removed = sync_event_poll_.remove_event(eventGUID);
  } else if (eventGUID.event_poll_id == ASYNC_EVENT_POOL_ID) {
    async_mutex_.lock();
    removed = async_event_poll_.remove_event(eventGUID);
    async_mutex_.unlock();
  } else {
    return false;
  }
  return removed;
}

void EventManager::sync_poll() { sync_event_poll_.poll(); }

void EventManager::start() {
  async_state_ = State::RUNNING;
  async_thread_ = std::thread(&EventManager::async_poll, this);
}

void EventManager::join() { async_thread_.join(); }

void EventManager::terminate() {
  if (async_state_ == State::WAITING) {
    async_state_ = State::TERMINATE;
    asyn_condition_.notify_one();
  }
  async_state_ = State::TERMINATE;
}

void EventManager::async_poll() {
  std::unique_lock<std::mutex> lock(async_mutex_);
  while (async_state_ != State::TERMINATE) {
    if (async_event_poll_.get_state() == EventPoll::State::NO_EVENTS) {
      async_state_ = State::WAITING;
      asyn_condition_.wait(lock);
    } else if (async_event_poll_.get_state() == EventPoll::State::DELAYED_EVENTS) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } else if (async_event_poll_.get_state() ==
               EventPoll::State::READY_TO_EXECUTE) {
      if (async_state_ == State::TERMINATE) return;

      lock.unlock();
      try {
        async_event_poll_.poll();
      } catch (...) {
        // TODO
        // Find way to do it
        // Cant use logger here, becouse it's using asyn poll
        // FW_G_LOGGER_DEBUG_TRACE("Unknow exeption occured.");
      }
      lock.lock();
    }
  }
}
}  // namespace FW::Thread
