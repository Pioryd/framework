#include "../platform.h"

#include <libgen.h>
#include <linux/limits.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

namespace FW::Platform {
std::string name() { return "Linux"; }

Time::ticks_t getFileModificationTime(std::string file) {
  struct stat attrib;
  if (stat(file.c_str(), &attrib) == 0) return attrib.st_mtime;
  return 0;
}

std::wstring getExecutableDirectory() {
  char result[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
  if (count != -1) {
    const auto path = std::string(dirname(result));
    return std::wstring(path.begin(), path.end());
  }
  return L"/";
}
}  // namespace FW::Platform
