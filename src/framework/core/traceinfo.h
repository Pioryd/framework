#ifndef FW_CORE_DEBUGINFO_H
#define FW_CORE_DEBUGINFO_H

#include "declarations.h"

namespace FW::Core {
struct TraceInfo {
  TraceInfo(const std::string& traceInfo) : trace_info_{traceInfo} {
    if (trace_info_ != "") trace_info_ = "[" + trace_info_ + "] ";
  }
  virtual ~TraceInfo() = default;

 protected:
  std::string trace_info_;
};
}  // namespace FW::Core
#endif  // #ifndef FW_CORE_DEBUGINFO_H