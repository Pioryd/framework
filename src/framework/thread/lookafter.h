#ifndef FW_THREAD_LOOKAFTER_H
#define FW_THREAD_LOOKAFTER_H

#include "declarations.h"

namespace FW::Thread {
template <typename T>
class LookAfter {
 public:
  LookAfter<T>(std::unique_ptr<T>& instance) : object_{std::move(instance)} {}
  LookAfter<T>()
      : object_{std::make_unique<T>()},
        main_thread_id_{std::this_thread::get_id()} {}

  LookAfter<T>(const LookAfter<T>&) = delete;
  LookAfter<T>& operator=(const LookAfter<T>&) = delete;

  T* force() const  { return object_.get(); }

  T* operator->() const {
    if (main_thread_id_ != std::this_thread::get_id())
      throw std::runtime_error(
          ("Called from another thread: " + std::string(typeid(T).name()))
              .c_str());
    return object_.get();
  }

 protected:
  std::unique_ptr<T> object_;
  const std::thread::id main_thread_id_;
};
}  // namespace FW::Thread
#endif  // #ifndef FW_THREAD_LOOKAFTER_H
