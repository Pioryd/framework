#include "../platform.h"

#ifdef FW_PLATFORM_ANDROID
namespace FW::Platform {
std::string name() { return "Android"; }

Time::ticks_t getFileModificationTime(std::string file) {}

std::wstring getExecutableDirectory() {}
}  // namespace FW::Platform
#endif  // #ifdef FW_PLATFORM_LINUX