#include "sqlmanager.h"

#include "../core/logger.h"
#include "../global.h"

#include <mysql/errmsg.h>

namespace FW::Database {
SqlManager::SqlManager() : transactionState_{TransactionState::NONE} {}

bool SqlManager::connect(const SqlConfig& sqlConfig) {
  FW_DEBUG_INSTRUCTIONS(G::Logger.debug(
      "SQL config:"
      "\n  Host: [" +
      sqlConfig.host + "]\n  User: [" + sqlConfig.user + "]\n  Passwrd: [" +
      sqlConfig.password + "]\n  Database name: [" + sqlConfig.databaseName +
      "]\n  Port: [" + std::to_string(sqlConfig.port) + "]\n  Socket: [" +
      sqlConfig.socket + "]\n");)

  sqlConfig_ = sqlConfig;

  return true;
}

Result::Result(void* handle) : handle_{handle} {}

size_t Result::getColumnIndex(const std::string& columnName) const {
  auto it = fieldNames_.find(columnName);
  if (it == fieldNames_.end())
    throw std::runtime_error(
        std::string("Column [" + columnName + "] does not exist.").c_str());
  return it->second;
}
}  // namespace FW::Database
