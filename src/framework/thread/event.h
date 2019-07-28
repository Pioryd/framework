#ifndef FW_THREAD_EVENT_H
#define FW_THREAD_EVENT_H

#include "declarations.h"

#include "../time/declarations.h"

#include <condition_variable>

namespace FW::Thread {
// Its ide thats indetifu event for mall threads
struct EventGUID {
  uint32_t id{0};
  Time::ticks_t timeOfIdSet{0};
  uint32_t eventPollId{0};
  // TODO
  // eventPollId can be 0 or not ?
  bool isSet() {
    if (id != 0 && timeOfIdSet != 0) return true;
  }
  void reset() { *this = {}; }
};

class Event {
 public:
  struct Compare {
    bool operator()(const Event_ptr& lArg, const Event_ptr& rArg) {
      return lArg->executeTime > rArg->executeTime;
    }
  };

 public:
  static constexpr EventGUID FATAL_GUID{0, 0, 0};
  static constexpr Time::ticks_t NO_DELAY{0};
  static constexpr int32_t REPEAT_ONCE{1};
  static constexpr int32_t REPEAT_ALL_THE_TIME{0};
  static constexpr int32_t MINIMUM_WAIT_TIME{10};
  static constexpr Time::ticks_t DO_NOT_EXPIRE{0};

 public:
  Event();
  virtual ~Event() = default;

 public:
  virtual void execute(Time::ticks_t syncTime);
  void cancel();

 protected:
  bool canExecuteRepeat(Time::ticks_t syncTime) const;

 private:  // Const at set
  EventGUID guid;
  std::function<void()> callback;
  Time::ticks_t executeTime;
  Time::ticks_t expirationTime;
  Time::ticks_t waitTimeBetweenRepeats;
  int32_t maxExecuteRepeats;

 private:  // Work values
  bool active;
  uint32_t executeCount;

 private:
  friend class EventPoll;
};

}  // namespace FW::Thread
#endif  // #ifndef FW_THREAD_EVENT_H
