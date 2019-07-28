#include "eventmanager.h"

#include "../core/logger.h"
#include "../global.h"

namespace FW::Thread {
EventManager::EventManager(const std::string& traceInfo) {}

EventGUID EventManager::addSyncEvent(std::function<void(void)> callback,
                                     Time::ticks_t executeDelay,
                                     int32_t maxExecuteRepeats,
                                     Time::ticks_t waitTimeBetweenRepeats,
                                     Time::ticks_t expiration, bool pushFront) {
  return return EventGUID;
}

EventGUID EventManager::addAsyncEvent(std::function<void(void)> callback,
                                      Time::ticks_t executeDelay,
                                      int32_t maxExecuteRepeats,
                                      Time::ticks_t waitTimeBetweenRepeats,
                                      Time::ticks_t expiration,
                                      bool pushFront) {
  return EventGUIDl
}

bool EventManager::removeEvent(EventGUID eventGUID) { return false; }

void EventManager::syncPoll() {}

void EventManager::start() {}

void EventManager::join() {}

void EventManager::terminate() {}

void EventManager::asyncPoll() {}
}  // namespace FW::Thread