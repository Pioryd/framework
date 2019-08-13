#ifndef FW_CORE_AUTO_ID_H
#define FW_CORE_AUTO_ID_H

#include "declarations.h"

namespace FW::Core {
// Thread safe
template <typename T>
class AutoId {
 public:
  AutoId(int32_t size) { ids_.resize(size); }
  int32_t lock(std::shared_ptr<T> object) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    for (int32_t i = 0; i < ids_.size(); i++) {
      if (ids_[i].expired()) {
        ids_[i] = object;
        return i;
      }
    }
    ids_.push_back(object);
    return ids_.size() - 1;
  }
  void release(int32_t id) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (id < ids_.size()) ids_[id].reset();
  }
  std::shared_ptr<T> get_object(int32_t id) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (id >= ids_.size()) return nullptr;
    if (auto object = ids_[id].lock()) return object;
    return nullptr;
  }

 protected:
  std::recursive_mutex mutex_;
  std::vector<std::weak_ptr<T>> ids_;
};
}  // namespace FW::Core
#endif  // #ifndef FW_CORE_AUTO_ID_H