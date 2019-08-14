#ifndef FW_CORE_SIGNAL_H
#define FW_CORE_SIGNAL_H

#include "declarations.h"

#include "../global.h"
#include "../pymodule/manager.h"

#define FW_CORE_SIGNAL(signalId, functionTypes) \
  FW::Core::Signal<std::function<functionTypes>> signalId { #signalId }

namespace FW::Core {
template <typename T>
class Signal {
 public:
  Signal(const std::string& pySignalId) : signal_id_{pySignalId} {
    signals[pySignalId] = this;
  }

  bool connect(const std::string& cpp_callback_id, T cpp_callback) {
    if (cpp_callbacks_.count(cpp_callback_id)) return false;
    cpp_callbacks_[cpp_callback_id] = cpp_callback;
    return true;
  }
  template <typename Obj, typename... Args>
  bool connect(const std::string& cpp_callback_id,
               void (Obj::*objectFunction)(Args...), Obj* object) {
    std::function<void(Args...)> function = [object,
                                             objectFunction](auto&&... args) {
      (object->*objectFunction)(args...);
    };

    if (cpp_callbacks_.count(cpp_callback_id)) return false;
    cpp_callbacks_[cpp_callback_id] = function;
    return true;
  }

  void disconnect(const std::string& callback_id) {
    cpp_callbacks_.erase(callback_id);
  }

  template <typename... Args>
  void send(Args... args) {
    for (auto& cpp_callbacks_pair : cpp_callbacks_) {
      auto& cpp_callback = cpp_callbacks_pair.second;
      cpp_callback(std::forward<Args>(args)...);
    }
    G::PyModule_Manager->signalManager.send(signal_id_,
                                           std::forward<Args>(args)...);
  }

 public:
  static inline std::map<std::string, Signal*> signals;  // <signal_id, signal>

 protected:
  std::string signal_id_;
  std::map<std::string, T> cpp_callbacks_;  // <callback_id, callback>
};
}  // namespace FW::Core
#endif  // #ifndef FW_CORE_SIGNAL_H