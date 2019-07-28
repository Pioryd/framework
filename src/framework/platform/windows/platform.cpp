#include "../platform.h"


#ifdef FW_PLATFORM_WINDOWS
namespace FW::Platform {
std::string name() { return "Windows"; }

Time::ticks_t getFileModificationTime(std::string file) {}

std::wstring getExecutableDirectory() {}
}  // namespace FW::Platform
#endif  // #ifdef FW_PLATFORM_WINDOWS