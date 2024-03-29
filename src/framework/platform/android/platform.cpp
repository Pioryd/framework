#include "../platform.h"

#ifdef FW_PLATFORM_ANDROID
namespace FW::Platform {
std::string name() { return "Android"; }

Time::ticks_t get_file_modification_time(std::string file) {
  // TODO
}

std::wstring get_executable_directory() {
  char result[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
  if (count != -1) {
    const auto path = std::string(dirname(result));
    return std::wstring(path.begin(), path.end());
  }
  return L"/";
}
}  // namespace FW::Platform
#endif  // #ifdef FW_PLATFORM_LINUX