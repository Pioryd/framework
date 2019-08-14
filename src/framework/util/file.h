#ifndef FW_IO_FILE_H
#define FW_IO_FILE_H

#include "declarations.h"

#include "../time/declarations.h"

namespace FW::IO {
namespace File {
Time::ticks_t get_file_modification_time(const std::string& fileName);
std::wstring get_executable_directory();
}  // namespace File
}  // namespace FW::IO
#endif  // FW_IO_FILE_H