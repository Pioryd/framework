#include "mysqlmanager.h"

#include "../core/logger.h"
#include "../global.h"

#include <mysql/errmsg.h>
#include <boost/lexical_cast.hpp>

namespace FW::Database {
MySqlManager::MySqlManager() : SqlManager(), handle_{nullptr} {}

MySqlManager::~MySqlManager() {
  if (transactionState_ == TransactionState::BEGIN) rollbackTransaction();
  if (handle_ != nullptr) mysql_close(handle_);
}

bool MySqlManager::connect(const SqlConfig& sqlConfig) {
  SqlManager::connect(sqlConfig);

  handle_ = mysql_init(nullptr);
  if (!handle_) {
    G::Logger.error("Failed to prepare and initialize the MYSQL database[" +
                    sqlConfig.databaseName + "].");
    return false;
  }

  constexpr bool reconnect = true;
  mysql_options(handle_, MYSQL_OPT_RECONNECT, &reconnect);

  if (!mysql_real_connect(handle_, sqlConfig.host.c_str(),
                          sqlConfig.user.c_str(), sqlConfig.password.c_str(),
                          sqlConfig.databaseName.c_str(), sqlConfig.port,
                          sqlConfig.socket.c_str(), 0)) {
    G::Logger.error("Mysql error(" + std::string(mysql_error(handle_)) +
                    "). Failed to establish the connection to the database[" +
                    sqlConfig.databaseName + "].");
    return false;
  }

  return true;
}

Result_sptr MySqlManager::execute(const std::string& query) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);

  MYSQL_RES* resultHandle = nullptr;

  FW_DEBUG_INSTRUCTIONS(
      G::Logger.debug("query(" + sqlConfig_.databaseName + "): " + query);)

  for (uint32_t i = 0; i < QUERY_ATTEMPTS; i++) {
    if (internalExecute(query) == false)
      throw std::runtime_error("Execute query failed.");

    resultHandle = mysql_store_result(handle_);
    FW_DEBUG_INSTRUCTIONS(
        G::Logger.debug("execute - result handle is " +
                        std::string((!resultHandle) ? "" : "not ") + "null");)
    if (resultHandle == nullptr) {
      std::string errorMessage = mysql_error(handle_);
      if (!errorMessage.empty()) {
        G::Logger.error("[mysql_store_result] error message: " + errorMessage +
                        ". Query(" + sqlConfig_.databaseName + "): " + query);

        if (isConnectionError(mysql_errno(handle_))) {
          FW_DEBUG_INSTRUCTIONS(
              G::Logger.debug("Connection error, try to execute again...");)
          continue;
        } else {
          throw std::runtime_error("Failed to get stored result.");
        }
      }
      return nullptr;
    }
    break;
  }

  Result_sptr result = std::make_shared<MySqlResult>(resultHandle);
  if (!result->next()) {
    FW_DEBUG_INSTRUCTIONS(if (config.debug_enabled)
                              G::Logger.debug("execute - no results.");)
    return nullptr;
  } else {
    FW_DEBUG_INSTRUCTIONS(if (config.debug_enabled)
                              G::Logger.debug("execute - there are results");)
  }

  return result;
}

bool MySqlManager::beginTransaction() {
  if (transactionState_ != TransactionState::NONE) return false;
  if (!execute("BEGIN")) { return false; }

  mutex_.lock();
  transactionState_ = TransactionState::BEGIN;
  return true;
}

bool MySqlManager::commitTransaction() {
  if (transactionState_ != TransactionState::BEGIN) { return false; }
  if (mysql_commit(handle_) != 0) {
    G::Logger.debug("Commit transaction error: " +
                    std::string(mysql_error(handle_)));
    transactionState_ = TransactionState::FAIL;
    return false;
  }

  transactionState_ = TransactionState::NONE;
  mutex_.unlock();
  return true;
}

bool MySqlManager::rollbackTransaction() {
  if (transactionState_ != TransactionState::BEGIN) { return false; }
  if (mysql_rollback(handle_) != 0) {
    G::Logger.debug("Rollback transaction error: " +
                    std::string(mysql_error(handle_)));
    transactionState_ = TransactionState::FAIL;
    return false;
  }

  transactionState_ = TransactionState::NONE;
  mutex_.unlock();
  return true;
}

std::string MySqlManager::escapeNumber(int64_t number) const {
  return escapeString(std::to_string(number));
}

std::string MySqlManager::escapeString(const std::string& s) const {
  return escapeBlob(s.c_str(), s.length());
}

std::string MySqlManager::escapeBlob(const char* s, uint32_t length) const {
  if (length == 0) return "''";

  // NOTE: I'm not sure that's correct.
  // [string * 2] in case when all charater will escaped
  // [+2] for two quotes
  // [+1] for null-terminate string
  auto maxSizeOfC_string = (length * 2) + 2 + 1;

  std::string escaped;
  escaped += '\'';
  if (length != 0) {
    char* output = new char[maxSizeOfC_string];
    mysql_real_escape_string(handle_, output, s, length);
    escaped.append(output);
    delete[] output;
  }
  escaped += '\'';

  return escaped;
}

bool MySqlManager::optimizeTables() {
  std::ostringstream query;
  query << "SELECT `TABLE_NAME` FROM `information_schema`.`tables` WHERE "
        << "`TABLE_SCHEMA` = " << escapeString(sqlConfig_.databaseName)
        << " AND `DATA_FREE` > 0;";

  try {
    Result_sptr result = execute(query.str());
    if (!result) return false;

    do {
      query.str("");
      query << "OPTIMIZE TABLE `" << result->getString("TABLE_NAME") << "`;";
      static_cast<void>(execute(query.str()));
    } while (result->next());

    return true;
  } catch (std::exception e) { G::Logger.error(e.what()); }
  return false;
}

bool MySqlManager::triggerExists(std::string trigger) {
  std::ostringstream query;
  query << "SELECT `TRIGGER_NAME` FROM `information_schema`.`triggers` WHERE "
           "`TRIGGER_SCHEMA` = "
        << escapeString(sqlConfig_.databaseName)
        << " AND `TRIGGER_NAME` = " << escapeString(trigger) << ";";

  Result_sptr result;
  if (!(result = execute(query.str()))) return false;

  return true;
}

bool MySqlManager::tableExists(std::string table) {
  std::ostringstream query;
  query << "SELECT `TABLE_NAME` FROM `information_schema`.`tables` WHERE "
           "`TABLE_SCHEMA` = "
        << escapeString(sqlConfig_.databaseName)
        << " AND `TABLE_NAME` = " << escapeString(table) << ";";

  Result_sptr result;
  if (!(result = execute(query.str()))) return false;

  return true;
}

bool MySqlManager::databaseExists() {
  std::ostringstream query;
  query << "SELECT `TABLE_NAME` FROM `information_schema`.`tables` WHERE "
           "`TABLE_SCHEMA` = "
        << escapeString(sqlConfig_.databaseName) << ";";

  Result_sptr result;
  if (!(result = execute(query.str()))) return false;

  return true;
}

std::string MySqlManager::getSqlClientVersion() const {
  return mysql_get_client_info();
}

int64_t MySqlManager::getLastInsertId() const {
  return static_cast<uint64_t>(mysql_insert_id(handle_));
}

bool MySqlManager::isConnectionError(uint32_t errorCode) {
  if ((errorCode != CR_SERVER_LOST) && (errorCode != CR_SERVER_GONE_ERROR) &&
      (errorCode != CR_CONN_HOST_ERROR) &&
      (errorCode != 1053 /*server shutdown*/) &&
      (errorCode != CR_CONNECTION_ERROR)) {
    return false;
  }
  FW_DEBUG_INSTRUCTIONS(
      G::Logger.debug("Connection error: " + std::to_string(errorCode));)
  return true;
}

bool MySqlManager::internalExecute(const std::string& query) {
  while (mysql_real_query(handle_, query.c_str(), query.length()) != MYSQL_OK) {
    G::Logger.error("Mysql error(" + std::string(mysql_error(handle_)) +
                    "). Failed to execute query[" + sqlConfig_.databaseName +
                    "]: " + query);

    if (isConnectionError(mysql_errno(handle_)) == false) return false;

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return true;
}

MySqlResult::MySqlResult(void* mySqlResult) {}

MySqlResult::~MySqlResult() {}

int64_t MySqlResult::getNumber(const std::string& columnName) const {
  return 0;
}

std::string MySqlResult::getString(const std::string& columnName) const {
  return "";
}

const char* MySqlResult::getStream(const std::string& columnName,
                                   unsigned long& size) const {
  return "";
}

bool MySqlResult::next() { return false; }
}  // namespace FW::Database
