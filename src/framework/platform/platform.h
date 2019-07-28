#ifndef FW_PLATFORM_PLATFORM_H
#define FW_PLATFORM_PLATFORM_H

#include "declarations.h"

#include "../time/declarations.h"

namespace FW::Platform {
std::string name();
Time::ticks_t getFileModificationTime(std::string file);
std::wstring getExecutableDirectory();
}  // namespace FW::Platform
#endif  // FW_PLATFORM_PLATFORM_H
