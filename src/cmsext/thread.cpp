#include "app.h"

#include <framework/global.h>
#include <framework/thread/eventmanager.h>

#include <framework/pymodule/manager.h>
#include <pybind11/embed.h>

namespace cmsext::thread {
void add_sync_event(std::function<void(void)> callback, int64_t execute_delay,
                    int64_t max_execute_repeats,
                    int64_t wait_time_between_repeats, int64_t expiration,
                    bool push_front) {
  FW::G::EventManager.add_sync_event(callback, execute_delay, max_execute_repeats,
                                   wait_time_between_repeats, expiration,
                                   push_front);
}

void add_async_event(std::function<void(void)> callback, int64_t execute_delay,
                     int64_t max_execute_repeats,
                     int64_t wait_time_between_repeats, int64_t expiration,
                     bool push_front) {
  FW::G::EventManager.add_async_event(
      callback, execute_delay, max_execute_repeats, wait_time_between_repeats,
      expiration, push_front);
}
}  // namespace cmsext::thread

PYBIND11_EMBEDDED_MODULE(cmsext_thread, m) {
  m.def(
      "add_sync_event",
      [](std::function<void(void)> callback,
         int64_t execute_delay = FW::Thread::Event::NO_DELAY,
         int64_t max_execute_repeats = FW::Thread::Event::REPEAT_ONCE,
         int64_t wait_time_between_repeats =
             FW::Thread::Event::MINIMUM_WAIT_TIME,
         int64_t expiration = FW::Thread::Event::DO_NOT_EXPIRE,
         bool push_front = false) {
        FW::G::EventManager.add_sync_event(
            callback, execute_delay, max_execute_repeats,
            wait_time_between_repeats, expiration, push_front);
      },
      pybind11::arg("callback"),
      pybind11::arg("execute_delay") = FW::Thread::Event::NO_DELAY,
      pybind11::arg("max_execute_repeats") = FW::Thread::Event::REPEAT_ONCE,
      pybind11::arg("wait_time_between_repeats") =
          FW::Thread::Event::MINIMUM_WAIT_TIME,
      pybind11::arg("expiration") = FW::Thread::Event::DO_NOT_EXPIRE,
      pybind11::arg("push_front") = false),
      m.def(
          "add_async_event",
          [](std::function<void(void)> callback,
             int64_t execute_delay = FW::Thread::Event::NO_DELAY,
             int64_t max_execute_repeats = FW::Thread::Event::REPEAT_ONCE,
             int64_t wait_time_between_repeats =
                 FW::Thread::Event::MINIMUM_WAIT_TIME,
             int64_t expiration = FW::Thread::Event::DO_NOT_EXPIRE,
             bool push_front = false) {
            FW::G::EventManager.add_async_event(
                callback, execute_delay, max_execute_repeats,
                wait_time_between_repeats, expiration, push_front);
          },
          pybind11::arg("callback"),
          pybind11::arg("execute_delay") = FW::Thread::Event::NO_DELAY,
          pybind11::arg("max_execute_repeats") = FW::Thread::Event::REPEAT_ONCE,
          pybind11::arg("wait_time_between_repeats") =
              FW::Thread::Event::MINIMUM_WAIT_TIME,
          pybind11::arg("expiration") = FW::Thread::Event::DO_NOT_EXPIRE,
          pybind11::arg("push_front") = false);
}