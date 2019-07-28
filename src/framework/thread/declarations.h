#ifndef FW_THREAD_DECLARATIONS_H
#define FW_THREAD_DECLARATIONS_H

#include "../pch.h"

namespace FW::Thread {
class Event;
class EventPoll;
class EventManager;

typedef std::shared_ptr<Event> Event_ptr;
typedef std::shared_ptr<EventPoll> EventPoll_ptr;
typedef std::shared_ptr<EventManager> EventManager_ptr;
}  // namespace FW::Thread
#endif  // #ifndef FW_THREAD_DECLARATIONS_H
