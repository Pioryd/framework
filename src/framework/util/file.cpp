#include "file.h"
#include "../platform/platform.h"

#include <cstring>

namespace FW::IO {
Time::ticks_t File::get_file_modification_time(const std::string& fileName) {
  return Platform::get_file_modification_time(fileName);
}
std::wstring File::get_executable_directory() {
  return Platform::get_executable_directory();
}
}  // namespace FW::IO