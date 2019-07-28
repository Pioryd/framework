#include "file.h"
#include "../platform/platform.h"

#include <cstring>

namespace FW::IO {
Time::ticks_t File::getFileModificationTime(const std::string& fileName) {
  return Platform::getFileModificationTime(fileName);
}
std::wstring File::getExecutableDirectory() {
  return Platform::getExecutableDirectory();
}
}  // namespace FW::IO