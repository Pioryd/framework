#ifndef FW_CMSEXT_THREAD_H
#define FW_CMSEXT_THREAD_H

#include <cmsext/declarations.h>

namespace cmsext::thread {
void add_sync_event(
    std::function<void(void)> callback,
    int64_t execute_delay = FW::Thread::Event::NO_DELAY,
    int64_t max_execute_repeats = FW::Thread::Event::REPEAT_ONCE,
    int64_t wait_time_between_repeats = FW::Thread::Event::MINIMUM_WAIT_TIME,
    int64_t expiration = FW::Thread::Event::DO_NOT_EXPIRE,
    bool push_front = false);

void add_async_event(
    std::function<void(void)> callback,
    int64_t execute_delay = FW::Thread::Event::NO_DELAY,
    int64_t max_execute_repeats = FW::Thread::Event::REPEAT_ONCE,
    int64_t wait_time_between_repeats = FW::Thread::Event::MINIMUM_WAIT_TIME,
    int64_t expiration = FW::Thread::Event::DO_NOT_EXPIRE,
    bool push_front = false);
}  // namespace cmsext::thread
#endif  // #ifndef FW_CMSEXT_THREAD_H
