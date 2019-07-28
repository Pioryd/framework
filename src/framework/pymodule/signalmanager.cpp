#include "signalmanager.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include "../core/logger.h"
#include "../global.h"
#include "module.h"

namespace FW::PyModule {
SignalManager::SignalManager() : state_{State::INITIALIZED} {}

bool SignalManager::connect(const std::string& signal, Callback_ptr callback) {
  FW_DEBUG_INSTRUCTIONS(
      G::Logger.debug("Connect to sginal: [" + signal + "]. Function: [" +
                      callback->getFunctionGlobalName() + "].");)

  if (isConnected(signal, callback)) return false;

  map_[signal].push_back(callback);
  return true;
}

bool SignalManager::disconnect(const std::string& signal,
                               Callback_ptr callback) {
  FW_DEBUG_INSTRUCTIONS(
      G::Logger.debug("Disonnect from sginal: [" + signal + "]. Function: [" +
                      callback->getFunctionGlobalName() + "].");)

  if (!contains(signal) || map_[signal].empty()) return false;

  bool found = false;
  auto& callbackList = map_[signal];
  auto callback_it = callbackList.begin();
  while (callback_it != callbackList.end()) {
    auto& listedCallback = (*callback_it);
    if ((*listedCallback) == (*callback)) {
      callback_it = callbackList.erase(callback_it);
      found = true;
    } else {
      ++callback_it;
    }
  }
  return found;
}

bool SignalManager::disconnect(Module_ptr& module) {
  if (module == nullptr) return false;

  bool found = false;
  for (auto& [signal, callbackList] : map_) {
    auto callback_it = callbackList.begin();
    while (callback_it != callbackList.end()) {
      if ((*callback_it)->getTopLevelModule().get() == module.get()) {
        callback_it = callbackList.erase(callback_it);
        found = true;
      } else {
        ++callback_it;
      }
    }
  }
  return found;
}

void SignalManager::terminate() {
  clear();
  state_ == State::TERMINATED;
}

bool SignalManager::isConnected(const std::string& signal,
                                Callback_ptr& callback) {
  if (!contains(signal)) return false;
  const auto& callbackList = map_[signal];
  for (const auto& mappedCallback : callbackList) {
    if ((*mappedCallback) == (*callback)) return true;
  }
  return false;
}

bool SignalManager::contains(const std::string& signal) const {
  return map_.count(signal);
}

bool SignalManager::empty() const { return map_.empty(); }

void SignalManager::clear() { map_.clear(); }
}  // namespace FW::PyModule
