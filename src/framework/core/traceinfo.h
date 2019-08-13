#ifndef FW_CORE_DEBUGINFO_H
#define FW_CORE_DEBUGINFO_H

#include "declarations.h"

namespace FW::Core {
struct TraceInfo {
  TraceInfo(const std::string& traceInfo) : _trace_info{traceInfo} {
    if (_trace_info != "") _trace_info = "[" + _trace_info + "] ";
  }
  virtual ~TraceInfo() = default;

 protected:
  std::string _trace_info;
};
}  // namespace FW::Core
#endif  // #ifndef FW_CORE_DEBUGINFO_H