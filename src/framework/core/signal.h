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
  Signal(const std::string& pySignalId) : pySignalId_{pySignalId} {}

  bool connect(const std::string& id, T callbackCpp) {
    if (callbacks_.count(id)) return false;
    callbacks_[id] = callbackCpp;
    return true;
  }
  template <typename Obj, typename... Args>
  bool connect(const std::string& id, void (Obj::*objectFunction)(Args...),
               Obj* object) {
    std::function<void(Args...)> function = [object,
                                             objectFunction](auto&&... args) {
      (object->*objectFunction)(args...);
    };

    if (callbacks_.count(id)) return false;
    callbacks_[id] = function;
    return true;
  }

  void disconnect(const std::string& id) { callbacks_.erase(id); }

  template <typename... Args>
  void send(Args... args) {
    for (auto& callback_pair : callbacks_) {
      auto& callback = callback_pair.second;
      callback(std::forward<Args>(args)...);
    }
    G::PyModuleManager->signalManager.send(pySignalId_,
                                           std::forward<Args>(args)...);
  }

 protected:
  std::string pySignalId_;
  std::map<std::string, T> callbacks_;
};
}  // namespace FW::Core
#endif  // #ifndef FW_CORE_SIGNAL_H