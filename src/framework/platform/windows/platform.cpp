#include "../platform.h"

#include <winsock2.h>
#include <windows.h>

#include <boost/algorithm/string.hpp>

#ifdef FW_PLATFORM_WINDOWS
namespace FW::Platform {
std::string name() { return "Windows"; }

Time::ticks_t getFileModificationTime(std::string file) {
  boost::replace_all(file, "/", "\\");
  std::wstring wfile(file.begin(), file.end());
  WIN32_FILE_ATTRIBUTE_DATA fileAttrData;
  memset(&fileAttrData, 0, sizeof(fileAttrData));
  GetFileAttributesExW(wfile.c_str(), GetFileExInfoStandard, &fileAttrData);
  ULARGE_INTEGER uli;
  uli.LowPart = fileAttrData.ftLastWriteTime.dwLowDateTime;
  uli.HighPart = fileAttrData.ftLastWriteTime.dwHighDateTime;
  return uli.QuadPart;
}

std::wstring getExecutableDirectory() {
  wchar_t exePath[MAX_PATH];
  GetModuleFileNameW(nullptr, exePath, MAX_PATH);
  const auto executableDir = std::wstring(exePath);
  const auto pos = executableDir.find_last_of('\\');
  if (pos != std::string::npos) return executableDir.substr(0, pos);
  return L"\\";
}
}  // namespace FW::Platform
#endif  // #ifdef FW_PLATFORM_WINDOWS
