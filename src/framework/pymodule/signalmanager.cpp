#include "signalmanager.h"

namespace FW::PyModule {
SignalManager::SignalManager() {}

bool SignalManager::connect(const std::string& signal, Callback_ptr callback) {
  return false;
}

bool SignalManager::disconnect(const std::string& signal,
                               Callback_ptr callback) {
  return false;
}

bool SignalManager::disconnect(Module_ptr& module) {
  return false;
}

void SignalManager::terminate() {
}

bool SignalManager::isConnected(const std::string& signal,
                                Callback_ptr& callback) {
  return false;
}

bool SignalManager::contains(const std::string& signal) const {
  return false;
}

bool SignalManager::empty() const { return false; }

void SignalManager::clear() { }
}  // namespace FW::PyModule
