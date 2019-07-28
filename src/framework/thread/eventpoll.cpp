#include "eventpoll.h"

#include "../core/logger.h"
#include "../global.h"
#include "../time/time.h"

namespace FW::Thread {
EventPoll::EventPoll(uint32_t id, const std::string& traceInfo)
    : TraceInfo{traceInfo},
      id{id},
      state{State::NO_EVENTS},
      lastSetEventGUID{Event::FATAL_GUID},
      enabled{true},
      pollStartTime{0} {}

EventGUID EventPoll::addInstantEvent(std::function<void(void)> callback,
                                     bool pushFront) {
  if (!enabled) return Event::FATAL_GUID;

  Event_ptr event = createEvent(callback);

  if (pushFront == true)
    instantEventList.push_front(event);
  else
    instantEventList.push_back(event);

  return lastSetEventGUID;
}

EventGUID EventPoll::addDelayedEvent(std::function<void(void)> callback,
                                     Time::ticks_t executeDelay,
                                     int32_t maxExecuteRepeats,
                                     Time::ticks_t waitTimeBetweenRepeats,
                                     Time::ticks_t expiration) {
  if (!enabled) return Event::FATAL_GUID;

  Event_ptr event = createEvent(callback, executeDelay, maxExecuteRepeats,
                                waitTimeBetweenRepeats, expiration);

  delayedEventList.push_back(event);
  delayedEventQueue.push(event);

  return lastSetEventGUID;
}

bool EventPoll::removeEvent(EventGUID eventGUID) {
  if (!enabled) return false;

  if (eventGUID.id == Event::FATAL_GUID.id) return false;

  auto takenEventId_it = takenEventIds.find(eventGUID.id);
  if (takenEventId_it == takenEventIds.end())
    return false;
  else
    takenEventIds.erase(takenEventId_it);

  // There should be only unique ids, so we can leave if we found one
  for (auto event_it = instantEventList.begin();
       event_it != instantEventList.end(); event_it++) {
    if ((*event_it)->guid.id == eventGUID.id) {
      (*event_it)->cancel();
      instantEventList.erase(event_it);

      return true;
    }
  }
  for (auto event_it = delayedEventList.begin();
       event_it != delayedEventList.end(); event_it++) {
    if ((*event_it)->guid.id == eventGUID.id) {
      (*event_it)->cancel();
      delayedEventList.erase(event_it);

      return true;
    }
  }
  return false;
}

EventPoll::State EventPoll::getState() {
  if (!instantEventList.empty()) return State::READY_TO_EXECUTE;
  if (!delayedEventQueue.empty()) {
    Event_ptr delaydEvent = delayedEventQueue.top();
    if (delaydEvent->executeTime < Time::Now::getMillis())
      return State::READY_TO_EXECUTE;
    else
      return State::DELAYED_EVENTS;
  }
  return State::NO_EVENTS;
}

void EventPoll::shutdown() {
  if (!enabled) return;

  // We need to disable the EventPoll, to avoid adding new events.
  enabled = false;

  while (!instantEventList.empty()) poll();

  // We cant wait until the times comes to exetuce delayed events, so we have to
  // cancel them now.
  auto event_it = delayedEventList.begin();
  while (event_it != delayedEventList.end()) {
    (*event_it)->cancel();
    delayedEventList.erase(event_it);
  }

  while (!delayedEventQueue.empty()) {
    Event_ptr event = delayedEventQueue.top();
    event->cancel();
    delayedEventQueue.pop();
  }
}

void EventPoll::poll() {
  // For sync need we will take time one once per poll
  pollStartTime = Time::Now::getMillis();
  int executedEventsCount = 0;

  auto sizeOfDelayedEventQueue = delayedEventQueue.size();
  for (auto i = 0; (i < sizeOfDelayedEventQueue && !delayedEventQueue.empty());
       i++) {
    executedEventsCount++;
    if (isExecutedEventsLimitExceeded(executedEventsCount)) break;

    Event_ptr delaydEvent = delayedEventQueue.top();
    if (delaydEvent->executeTime > pollStartTime) break;

    delayedEventQueue.pop();

    if (delaydEvent->canExecuteRepeat(pollStartTime))
      delaydEvent->execute(pollStartTime);

    // Must check again, becouse execute counter was increased
    if (delaydEvent->canExecuteRepeat(pollStartTime)) {
      delayedEventQueue.push(delaydEvent);
    } else {
      // delaydEvent->cancel();
      delayedEventList.remove(delaydEvent);
    }
  }

  auto sizeOfInstantEventList = instantEventList.size();
  for (auto i = 0; (i < sizeOfInstantEventList && !instantEventList.empty());
       i++) {
    executedEventsCount++;
    if (isExecutedEventsLimitExceeded(executedEventsCount)) break;

    Event_ptr event = instantEventList.front();
    instantEventList.pop_front();
    event->execute(pollStartTime);
  }

  // FW_DEBUG_INSTRUCTIONS(
  //    static auto lastCountQueue = delayedEventQueue.size();
  //    static auto lastCountList = delayedEventList.size();
  //    if ((lastCountQueue != delayedEventQueue.size()) ||
  //        (lastCountList != delayedEventList.size())) {
  //      lastCountQueue = delayedEventQueue.size();
  //      lastCountList = delayedEventList.size();
  //      FW_G_LOGGER_DEBUG_TRACE(
  //          "Diffrence between delayedEventQueue and delayedEventList [" +
  //          std::to_string(delayedEventQueue.size()) + "/" +
  //          std::to_string(delayedEventList.size()) + "]");
  //    })
}

bool EventPoll::isExecutedEventsLimitExceeded(uint32_t executedEventsCount) {
  // FW_DEBUG_INSTRUCTIONS(
  //    static auto lastExecutedEventsCount = 0;
  //    if (executedEventsCount > lastExecutedEventsCount) {
  //      lastExecutedEventsCount = executedEventsCount;
  //      FW_G_LOGGER_DEBUG_TRACE(
  //          "Actual maximum number of executed events per poll is " +
  //          std::to_string(lastExecutedEventsCount));
  //    })

  if (executedEventsCount > 50) {
    auto takenTime = Time::Now::getMillis() - pollStartTime;
    // if (takenTime > 100)
    //  FW_G_LOGGER_ERROR_TRACE("EventPoll[" + std::to_string(id) +
    //                          "] take too long(" + std::to_string(takenTime) +
    //                          " ms). Instant events count(" +
    //                          std::to_string(instantEventList.size()) +
    //                          "). Delayed events count[list/queue](" +
    //                          std::to_string(delayedEventList.size()) + "/" +
    //                          std::to_string(delayedEventQueue.size()) +
    //                          ").");
    return true;
  }
  return false;
}

Event_ptr EventPoll::createEvent(std::function<void(void)> callback,
                                 Time::ticks_t executeDelay,
                                 int32_t maxExecuteRepeats,
                                 Time::ticks_t waitTimeBetweenRepeats,
                                 Time::ticks_t expiration) {
  // Try to get free id
  const uint32_t fatalLoopNumber = lastSetEventGUID.id;
  // We will iterate only by ID to make it faster
  lastSetEventGUID.timeOfIdSet = Time::Now::getMillis();
  lastSetEventGUID.eventPollId = id;
  while (true) {
    ++lastSetEventGUID.id;
    if (lastSetEventGUID.id == Event::FATAL_GUID.id) lastSetEventGUID.id = 1;

    auto it = takenEventIds.find(lastSetEventGUID.id);
    if (it == takenEventIds.end()) break;

    // If we loop over range of uint32_t, thats means no id is free
    if (lastSetEventGUID.id == fatalLoopNumber) return nullptr;
  }

  // Setup new event
  Event_ptr event = std::make_shared<Event>();
  event->guid = lastSetEventGUID;
  event->callback = callback;
  if (executeDelay == Event::NO_DELAY)
    event->executeTime = Event::NO_DELAY;
  else
    event->executeTime = Time::Now::getMillis() + executeDelay;
  event->maxExecuteRepeats = maxExecuteRepeats;
  event->waitTimeBetweenRepeats = waitTimeBetweenRepeats;
  if (expiration == Event::DO_NOT_EXPIRE)
    event->expirationTime = Event::DO_NOT_EXPIRE;
  else
    event->expirationTime = Time::Now::getMillis() + expiration;

  return event;
}
}  // namespace FW::Thread