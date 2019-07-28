#include "../platform.h"

namespace FW::Platform {
std::string name() { return "Linux"; }

Time::ticks_t getFileModificationTime(std::string file) {}

std::wstring getExecutableDirectory() {}
}  // namespace FW::Platform
