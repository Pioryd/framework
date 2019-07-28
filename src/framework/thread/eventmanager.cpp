#include "eventmanager.h"

#include "../core/logger.h"
#include "../global.h"

namespace FW::Thread {
EventManager::EventManager(const std::string& traceInfo)
    : TraceInfo{traceInfo},
      syncEventPoll{SYNC_EVENT_POOL_ID, traceInfo + " syncEventPoll"},
      asyncEventPoll{ASYNC_EVENT_POOL_ID, traceInfo + " asyncEventPoll"},
      asyncState{State::TERMINATE} {}

EventGUID EventManager::addSyncEvent(std::function<void(void)> callback,
                                     Time::ticks_t executeDelay,
                                     int32_t maxExecuteRepeats,
                                     Time::ticks_t waitTimeBetweenRepeats,
                                     Time::ticks_t expiration, bool pushFront) {
  EventGUID returnEventGUID = Event::FATAL_GUID;

  if (executeDelay == Event::NO_DELAY) {
    returnEventGUID = syncEventPoll.addInstantEvent(callback, pushFront);
  } else {
    returnEventGUID =
        syncEventPoll.addDelayedEvent(callback, executeDelay, maxExecuteRepeats,
                                      waitTimeBetweenRepeats, expiration);
  }

  return returnEventGUID;
}

EventGUID EventManager::addAsyncEvent(std::function<void(void)> callback,
                                      Time::ticks_t executeDelay,
                                      int32_t maxExecuteRepeats,
                                      Time::ticks_t waitTimeBetweenRepeats,
                                      Time::ticks_t expiration,
                                      bool pushFront) {
  EventGUID returnEventGUID = Event::FATAL_GUID;

  asyncMutex.lock();

  if (executeDelay == Event::NO_DELAY) {
    returnEventGUID = asyncEventPoll.addInstantEvent(callback, pushFront);
  } else {
    returnEventGUID = asyncEventPoll.addDelayedEvent(
        callback, executeDelay, maxExecuteRepeats, waitTimeBetweenRepeats,
        expiration);
  }

  if (asyncState == State::WAITING) {
    asyncState = State::RUNNING;
    asyncCondition.notify_one();
  }

  asyncMutex.unlock();

  return returnEventGUID;
}

bool EventManager::removeEvent(EventGUID eventGUID) {
  bool removed = false;
  if (eventGUID.eventPollId == SYNC_EVENT_POOL_ID) {
    removed = syncEventPoll.removeEvent(eventGUID);
  } else if (eventGUID.eventPollId == ASYNC_EVENT_POOL_ID) {
    asyncMutex.lock();
    removed = asyncEventPoll.removeEvent(eventGUID);
    asyncMutex.unlock();
  } else {
    return false;
  }
  return removed;
}

void EventManager::syncPoll() { syncEventPoll.poll(); }

void EventManager::start() {
  asyncState = State::RUNNING;
  asyncThread = std::thread(&EventManager::asyncPoll, this);
}

void EventManager::join() { asyncThread.join(); }

void EventManager::terminate() {
  if (asyncState == State::WAITING) {
    asyncState = State::TERMINATE;
    asyncCondition.notify_one();
  }
  asyncState = State::TERMINATE;
}

void EventManager::asyncPoll() {
  std::unique_lock<std::mutex> lock(asyncMutex);
  while (asyncState != State::TERMINATE) {
    if (asyncEventPoll.getState() == EventPoll::State::NO_EVENTS) {
      asyncState = State::WAITING;
      asyncCondition.wait(lock);
    } else if (asyncEventPoll.getState() == EventPoll::State::DELAYED_EVENTS) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } else if (asyncEventPoll.getState() ==
               EventPoll::State::READY_TO_EXECUTE) {
      if (asyncState == State::TERMINATE) return;

      lock.unlock();
      try {
        asyncEventPoll.poll();
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
