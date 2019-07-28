#include "sqlmanager.h"

#include <mysql/errmsg.h>

namespace FW::Database {
SqlManager::SqlManager() {}

bool SqlManager::connect(const SqlConfig& sqlConfig) {}

Result::Result(void* handle) {}

size_t Result::getColumnIndex(const std::string& columnName) const { return 0; }
}  // namespace FW::Database
