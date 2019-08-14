#include "eventpoll.h"

#include "../core/logger.h"
#include "../global.h"
#include "../time/time.h"

namespace FW::Thread {
EventPoll::EventPoll(uint32_t id, const std::string& trace_info)
    : TraceInfo{trace_info},
      id_{id},
      state_{State::NO_EVENTS},
      last_set_event_guid_{Event::FATAL_GUID},
      enabled_{true},
      poll_start_time_{0} {}

EventGUID EventPoll::add_instant_event(std::function<void(void)> callback,
                                     bool push_front) {
  if (!enabled_) return Event::FATAL_GUID;

  Event_ptr event = create_event(callback);

  if (push_front == true)
    instant_event_list_.push_front(event);
  else
    instant_event_list_.push_back(event);

  return last_set_event_guid_;
}

EventGUID EventPoll::add_delayed_event(std::function<void(void)> callback,
                                     Time::ticks_t execute_delay,
                                     int32_t max_execute_repeats,
                                     Time::ticks_t wait_time_between_repeats,
                                     Time::ticks_t expiration) {
  if (!enabled_) return Event::FATAL_GUID;

  Event_ptr event = create_event(callback, execute_delay, max_execute_repeats,
                                wait_time_between_repeats, expiration);

  delayed_event_list_.push_back(event);
  delayed_event_queue_.push(event);

  return last_set_event_guid_;
}

bool EventPoll::remove_event(EventGUID event_guid) {
  if (!enabled_) return false;

  if (event_guid.id == Event::FATAL_GUID.id) return false;

  auto taken_event_id_it = taken_event_ids_.find(event_guid.id);
  if (taken_event_id_it == taken_event_ids_.end())
    return false;
  else
    taken_event_ids_.erase(taken_event_id_it);

  // There should be only unique ids, so we can leave if we found one
  for (auto event_it = instant_event_list_.begin();
       event_it != instant_event_list_.end(); event_it++) {
    if ((*event_it)->guid_.id == event_guid.id) {
      (*event_it)->cancel();
      instant_event_list_.erase(event_it);

      return true;
    }
  }
  for (auto event_it = delayed_event_list_.begin();
       event_it != delayed_event_list_.end(); event_it++) {
    if ((*event_it)->guid_.id == event_guid.id) {
      (*event_it)->cancel();
      delayed_event_list_.erase(event_it);

      return true;
    }
  }
  return false;
}

EventPoll::State EventPoll::get_state() {
  if (!instant_event_list_.empty()) return State::READY_TO_EXECUTE;
  if (!delayed_event_queue_.empty()) {
    Event_ptr delayd_event = delayed_event_queue_.top();
    if (delayd_event->execute_time_ < Time::Now::get_millis())
      return State::READY_TO_EXECUTE;
    else
      return State::DELAYED_EVENTS;
  }
  return State::NO_EVENTS;
}

void EventPoll::shutdown() {
  if (!enabled_) return;

  // We need to disable the EventPoll, to avoid adding new events.
  enabled_ = false;

  while (!instant_event_list_.empty()) poll();

  // We cant wait until the times comes to exetuce delayed events, so we have to
  // cancel them now.
  auto event_it = delayed_event_list_.begin();
  while (event_it != delayed_event_list_.end()) {
    (*event_it)->cancel();
    delayed_event_list_.erase(event_it);
  }

  while (!delayed_event_queue_.empty()) {
    Event_ptr event = delayed_event_queue_.top();
    event->cancel();
    delayed_event_queue_.pop();
  }
}

void EventPoll::poll() {
  // For sync need we will take time one once per poll
  poll_start_time_ = Time::Now::get_millis();
  int executed_events_count = 0;

  auto size_of_delayed_event_queue = delayed_event_queue_.size();
  for (auto i = 0;
       (i < size_of_delayed_event_queue && !delayed_event_queue_.empty());
       i++) {
    executed_events_count++;
    if (is_executed_events_limit_exceeded(executed_events_count)) break;

    Event_ptr delayd_event = delayed_event_queue_.top();
    if (delayd_event->execute_time_ > poll_start_time_) break;

    delayed_event_queue_.pop();

    if (delayd_event->canExecuteRepeat(poll_start_time_))
      delayd_event->execute(poll_start_time_);

    // Must check again, becouse execute counter was increased
    if (delayd_event->canExecuteRepeat(poll_start_time_)) {
      delayed_event_queue_.push(delayd_event);
    } else {
      // delayd_event->cancel();
      delayed_event_list_.remove(delayd_event);
    }
  }

  auto size_of_instant_event_list = instant_event_list_.size();
  for (auto i = 0; (i < size_of_instant_event_list && !instant_event_list_.empty());
       i++) {
    executed_events_count++;
    if (is_executed_events_limit_exceeded(executed_events_count)) break;

    Event_ptr event = instant_event_list_.front();
    instant_event_list_.pop_front();
    event->execute(poll_start_time_);
  }

  // FW_DEBUG_INSTRUCTIONS(
  //    static auto lastCountQueue = delayed_event_queue.size();
  //    static auto lastCountList = delayed_event_list_.size();
  //    if ((lastCountQueue != delayed_event_queue.size()) ||
  //        (lastCountList != delayed_event_list_.size())) {
  //      lastCountQueue = delayed_event_queue.size();
  //      lastCountList = delayed_event_list_.size();
  //      FW_G_LOGGER_DEBUG_TRACE(
  //          "Diffrence between delayed_event_queue and delayed_event_list_ [" +
  //          std::to_string(delayed_event_queue.size()) + "/" +
  //          std::to_string(delayed_event_list_.size()) + "]");
  //    })
}

bool EventPoll::is_executed_events_limit_exceeded(uint32_t executed_events_count) {
  // FW_DEBUG_INSTRUCTIONS(
  //    static auto lastExecutedEventsCount = 0;
  //    if (executed_events_count > lastExecutedEventsCount) {
  //      lastExecutedEventsCount = executed_events_count;
  //      FW_G_LOGGER_DEBUG_TRACE(
  //          "Actual maximum number of executed events per poll is " +
  //          std::to_string(lastExecutedEventsCount));
  //    })

  if (executed_events_count > 50) {
    auto takenTime = Time::Now::get_millis() - poll_start_time_;
    // if (takenTime > 100)
    //  FW_G_LOGGER_ERROR_TRACE("EventPoll[" + std::to_string(id) +
    //                          "] take too long(" + std::to_string(takenTime) +
    //                          " ms). Instant events count(" +
    //                          std::to_string(instant_event_list_.size()) +
    //                          "). Delayed events count[list/queue](" +
    //                          std::to_string(delayed_event_list_.size()) + "/" +
    //                          std::to_string(delayed_event_queue.size()) +
    //                          ").");
    return true;
  }
  return false;
}

Event_ptr EventPoll::create_event(std::function<void(void)> callback,
                                 Time::ticks_t execute_delay,
                                 int32_t max_execute_repeats,
                                 Time::ticks_t wait_time_between_repeats,
                                 Time::ticks_t expiration) {
  // Try to get free id
  const uint32_t fatal_loop_number = last_set_event_guid_.id;
  // We will iterate only by ID to make it faster
  last_set_event_guid_.time_of_id_set = Time::Now::get_millis();
  last_set_event_guid_.event_poll_id = id_;
  while (true) {
    ++last_set_event_guid_.id;
    if (last_set_event_guid_.id == Event::FATAL_GUID.id) last_set_event_guid_.id = 1;

    auto it = taken_event_ids_.find(last_set_event_guid_.id);
    if (it == taken_event_ids_.end()) break;

    // If we loop over range of uint32_t, thats means no id is free
    if (last_set_event_guid_.id == fatal_loop_number) return nullptr;
  }

  // Setup new event
  Event_ptr event = std::make_shared<Event>();
  event->guid_ = last_set_event_guid_;
  event->callback_ = callback;
  if (execute_delay == Event::NO_DELAY)
    event->execute_time_ = Event::NO_DELAY;
  else
    event->execute_time_ = Time::Now::get_millis() + execute_delay;
  event->max_execute_repeats_ = max_execute_repeats;
  event->wait_time_between_repeats_ = wait_time_between_repeats;
  if (expiration == Event::DO_NOT_EXPIRE)
    event->expiration_time_ = Event::DO_NOT_EXPIRE;
  else
    event->expiration_time_ = Time::Now::get_millis() + expiration;

  return event;
}
}  // namespace FW::Thread