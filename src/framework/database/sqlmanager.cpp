#include "sqlmanager.h"

#include "../core/logger.h"
#include "../global.h"

#include <mysql/errmsg.h>

namespace FW::Database {
SqlManager::SqlManager() : transaction_state_{TransactionState::NONE} {}

bool SqlManager::connect(const SqlConfig& sql_config) {
  FW_DEBUG_INSTRUCTIONS(G::Logger.debug(
      "SQL config:"
      "\n  Host: [" +
      sql_config.host + "]\n  User: [" + sql_config.user + "]\n  Passwrd: [" +
      sql_config.password + "]\n  Database name: [" + sql_config.database_name +
      "]\n  Port: [" + std::to_string(sql_config.port) + "]\n  Socket: [" +
      sql_config.socket + "]\n");)

  sql_config_ = sql_config;

  return true;
}

Result::Result(void* handle) : handle_{handle} {}

size_t Result::get_column_index(const std::string& column_name) const {
  auto it = field_names_.find(column_name);
  if (it == field_names_.end())
    throw std::runtime_error(
        std::string("Column [" + column_name + "] does not exist.").c_str());
  return it->second;
}
}  // namespace FW::Database
