#ifndef FW_CORE_APPLICATIONBASE_H
#define FW_CORE_APPLICATIONBASE_H

#include "declarations.h"

namespace FW::Core {
class ApplicationBase {
 public:
  std::wstring getWorkingDirectory();
};
}  // namespace FW::Core
#endif  // #ifndef FW_CORE_APPLICATIONBASE_H