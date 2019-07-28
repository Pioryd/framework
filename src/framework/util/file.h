#ifndef FW_IO_FILE_H
#define FW_IO_FILE_H

#include "declarations.h"

#include "../time/declarations.h"

namespace FW::IO {
namespace File {
Time::ticks_t getFileModificationTime(const std::string& fileName);
std::wstring getExecutableDirectory();
}  // namespace File
}  // namespace FW::IO
#endif  // FW_IO_FILE_H