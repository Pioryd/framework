#ifndef FW_PYMODULE_SIGNALMANAGER_H
#define FW_PYMODULE_SIGNALMANAGER_H

#include "declarations.h"

#include "callback.h"

namespace FW::PyModule {
class SignalManager {
 public:
  enum class State { INITIALIZED, TERMINATED };

 public:
  SignalManager();

  SignalManager(const SignalManager&) = delete;
  SignalManager& operator=(const SignalManager&) = delete;

  struct Config {
    bool debug_enabled = true;
  };

  // Searches list of callbacks of given signal
  bool connect(const std::string& signal, Callback_ptr callback);
  // Searches list of callbacks of given signal
  bool disconnect(const std::string& signal, Callback_ptr callback);
  // Searches all signals and lists of callbacks
  bool disconnect(Module_ptr& module);

  void terminate();

  template <typename... Args>
  void send(const std::string& signal, const Args&... args);

  bool isConnected(const std::string& signal, Callback_ptr& callback);
  bool contains(const std::string& signal) const;
  bool empty() const;
  void clear();

 public:
  Config config;

 private:
  State state_;
  std::unordered_map<std::string, std::list<Callback_ptr>> map_;
};

template <typename... Args>
void SignalManager::send(const std::string& signal, const Args&... args) {
  if (!contains(signal) || map_[signal].empty()) return;

  if (state_ == State::TERMINATED) {
    FW::G::Logger.warning(
        "SignalManager - Unable to call callback. Signal manager is "
        "terminated.");
    return;
  }

  const auto& callbackList = map_[signal];
  for (const auto& callback : callbackList) callback->call(args...);
}
}  // namespace FW::PyModule
#endif  // #ifndef FW_PYMODULE_SIGNALMANAGER_H
