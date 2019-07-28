#ifndef FW_CORE_DEBUGINFO_H
#define FW_CORE_DEBUGINFO_H

#include "declarations.h"

namespace FW::Core {
struct TraceInfo {
  TraceInfo(const std::string& traceInfo) : traceInfo{traceInfo} {
    if (this->traceInfo != "") this->traceInfo = "[" + this->traceInfo + "] ";
  }
  virtual ~TraceInfo() = default;

 protected:
  std::string traceInfo;
};
}  // namespace FW::Core
#endif  // #ifndef FW_CORE_DEBUGINFO_H