#ifndef FW_CMSEXT_MODULE_H
#define FW_CMSEXT_MODULE_H

#include <cmsext/declarations.h>

#include <framework/core/signal.h>
#include <framework/pch.h>

namespace cmsext::module {
void load(const cms::string& module_name);
void reload(const cms::string& module_name);
void restart_modules();
void run_script(const cms::string& script);

template <typename T>
void connect(const cms::string& signal_id, T callbackCpp) {
  if (FW::Core::Signal<T>::signals.count((std::string)signal_id)) {
    std::stringstream callbackd_id;
    callbackd_id << (std::string)signal_id
                 << static_cast<const void*>(&callbackCpp);
    FW::Core::Signal<T>::signals[(std::string)signal_id]->connect(
        callbackd_id.str(), callbackCpp);
  }
};
template <typename Obj, typename... Args>
bool connect(const cms::string& signal_id, void (Obj::*objectFunction)(Args...),
             Obj* object) {
  if (FW::Core::Signal<T>::signals.count(signal_id)) {
    std::stringstream callbackd_id;
    callbackd_id << (std::string)signal_id
                 << static_cast<const void*>(objectFunction)
                 << static_cast<const void*>(object);

    FW::Core::Signal<T>::signals[(std::string)signal_id]->connect(
        callbackd_id.str(), objectFunction, object);
  }
}
/*
TODO
Test with std::bind, std::function and cpp pointers.
*/
template <typename T>
void disconnect(const cms::string& signal_id, T callbackCpp) {
  if (FW::Core::Signal<T>::signals.count(signal_id)) {
    std::stringstream callbackd_id;
    callbackd_id << signal_id << static_cast<const void*>(callbackCpp);

    FW::Core::Signal<T>::signals[signal_id]->disconnect(callbackd_id.str());
  }
}
}  // namespace cmsext::module

#endif  // #ifndef FW_CMSEXT_MODULE_H
