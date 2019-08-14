#include "mysqlmanager.h"

#include "../core/logger.h"
#include "../global.h"

#include <mysql/errmsg.h>
#include <boost/lexical_cast.hpp>

namespace FW::Database {
MySqlManager::MySqlManager() : SqlManager(), handle_{nullptr} {}

MySqlManager::~MySqlManager() {
  if (transaction_state_ == TransactionState::BEGIN) rollback_transaction();
  if (handle_ != nullptr) mysql_close(handle_);
}

bool MySqlManager::connect(const SqlConfig& sql_config) {
  SqlManager::connect(sql_config);

  handle_ = mysql_init(nullptr);
  if (!handle_) {
    G::Logger.error("Failed to prepare and initialize the MYSQL database[" +
                    sql_config.database_name + "].");
    return false;
  }

  constexpr bool reconnect = true;
  mysql_options(handle_, MYSQL_OPT_RECONNECT, &reconnect);

  if (!mysql_real_connect(handle_, sql_config.host.c_str(),
                          sql_config.user.c_str(), sql_config.password.c_str(),
                          sql_config.database_name.c_str(), sql_config.port,
                          sql_config.socket.c_str(), 0)) {
    G::Logger.error("Mysql error(" + std::string(mysql_error(handle_)) +
                    "). Failed to establish the connection to the database[" +
                    sql_config.database_name + "].");
    return false;
  }

  return true;
}

Result_sptr MySqlManager::execute(const std::string& query) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);

  MYSQL_RES* result_handle = nullptr;

  FW_DEBUG_INSTRUCTIONS(
      G::Logger.debug("query(" + sql_config_.database_name + "): " + query);)

  for (uint32_t i = 0; i < QUERY_ATTEMPTS; i++) {
    if (internal_execute(query) == false)
      throw std::runtime_error("Execute query failed.");

    result_handle = mysql_store_result(handle_);
    FW_DEBUG_INSTRUCTIONS(
        G::Logger.debug("execute - result handle is " +
                        std::string((!result_handle) ? "" : "not ") + "null");)
    if (result_handle == nullptr) {
      std::string errorMessage = mysql_error(handle_);
      if (!errorMessage.empty()) {
        G::Logger.error("[mysql_store_result] error message: " + errorMessage +
                        ". Query(" + sql_config_.database_name + "): " + query);

        if (is_connection_error(mysql_errno(handle_))) {
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

  Result_sptr result = std::make_shared<MySqlResult>(result_handle);
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

bool MySqlManager::begin_transaction() {
  if (transaction_state_ != TransactionState::NONE) return false;
  if (!execute("BEGIN")) { return false; }

  mutex_.lock();
  transaction_state_ = TransactionState::BEGIN;
  return true;
}

bool MySqlManager::commit_transaction() {
  if (transaction_state_ != TransactionState::BEGIN) { return false; }
  if (mysql_commit(handle_) != 0) {
    G::Logger.debug("Commit transaction error: " +
                    std::string(mysql_error(handle_)));
    transaction_state_ = TransactionState::FAIL;
    return false;
  }

  transaction_state_ = TransactionState::NONE;
  mutex_.unlock();
  return true;
}

bool MySqlManager::rollback_transaction() {
  if (transaction_state_ != TransactionState::BEGIN) { return false; }
  if (mysql_rollback(handle_) != 0) {
    G::Logger.debug("Rollback transaction error: " +
                    std::string(mysql_error(handle_)));
    transaction_state_ = TransactionState::FAIL;
    return false;
  }

  transaction_state_ = TransactionState::NONE;
  mutex_.unlock();
  return true;
}

std::string MySqlManager::escape_number(int64_t number) const {
  return escape_string(std::to_string(number));
}

std::string MySqlManager::escape_string(const std::string& s) const {
  return escape_blob(s.c_str(), s.length());
}

std::string MySqlManager::escape_blob(const char* s, uint32_t length) const {
  if (length == 0) return "''";

  // NOTE: I'm not sure that's correct.
  // [string * 2] in case when all charater will escaped
  // [+2] for two quotes
  // [+1] for null-terminate string
  auto max_size_of_c_string = (length * 2) + 2 + 1;

  std::string escaped;
  escaped += '\'';
  if (length != 0) {
    char* output = new char[max_size_of_c_string];
    mysql_real_escape_string(handle_, output, s, length);
    escaped.append(output);
    delete[] output;
  }
  escaped += '\'';

  return escaped;
}

bool MySqlManager::optimize_tables() {
  std::ostringstream query;
  query << "SELECT `TABLE_NAME` FROM `information_schema`.`tables` WHERE "
        << "`TABLE_SCHEMA` = " << escape_string(sql_config_.database_name)
        << " AND `DATA_FREE` > 0;";

  try {
    Result_sptr result = execute(query.str());
    if (!result) return false;

    do {
      query.str("");
      query << "OPTIMIZE TABLE `" << result->get_string("TABLE_NAME") << "`;";
      static_cast<void>(execute(query.str()));
    } while (result->next());

    return true;
  } catch (std::exception e) { G::Logger.error(e.what()); }
  return false;
}

bool MySqlManager::trigger_exists(std::string trigger) {
  std::ostringstream query;
  query << "SELECT `TRIGGER_NAME` FROM `information_schema`.`triggers` WHERE "
           "`TRIGGER_SCHEMA` = "
        << escape_string(sql_config_.database_name)
        << " AND `TRIGGER_NAME` = " << escape_string(trigger) << ";";

  Result_sptr result;
  if (!(result = execute(query.str()))) return false;

  return true;
}

bool MySqlManager::table_exists(std::string table) {
  std::ostringstream query;
  query << "SELECT `TABLE_NAME` FROM `information_schema`.`tables` WHERE "
           "`TABLE_SCHEMA` = "
        << escape_string(sql_config_.database_name)
        << " AND `TABLE_NAME` = " << escape_string(table) << ";";

  Result_sptr result;
  if (!(result = execute(query.str()))) return false;

  return true;
}

bool MySqlManager::database_exists() {
  std::ostringstream query;
  query << "SELECT `TABLE_NAME` FROM `information_schema`.`tables` WHERE "
           "`TABLE_SCHEMA` = "
        << escape_string(sql_config_.database_name) << ";";

  Result_sptr result;
  if (!(result = execute(query.str()))) return false;

  return true;
}

std::string MySqlManager::get_sql_client_version() const {
  return mysql_get_client_info();
}

int64_t MySqlManager::get_last_insert_id() const {
  return static_cast<uint64_t>(mysql_insert_id(handle_));
}

bool MySqlManager::is_connection_error(uint32_t errorCode) {
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

bool MySqlManager::internal_execute(const std::string& query) {
  while (mysql_real_query(handle_, query.c_str(), query.length()) != MYSQL_OK) {
    G::Logger.error("Mysql error(" + std::string(mysql_error(handle_)) +
                    "). Failed to execute query[" + sql_config_.database_name +
                    "]: " + query);

    if (is_connection_error(mysql_errno(handle_)) == false) return false;

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return true;
}

MySqlResult::MySqlResult(void* mySqlResult)
    : Result(mySqlResult), row_(nullptr) {
  size_t column_index = 0;
  MYSQL_FIELD* field = mysql_fetch_field(static_cast<MYSQL_RES*>(handle_));
  while (field) {
    field_names_[field->name] = column_index++;
    field = mysql_fetch_field(static_cast<MYSQL_RES*>(handle_));
  }
}

MySqlResult::~MySqlResult() {
  mysql_free_result(static_cast<MYSQL_RES*>(handle_));
}

int64_t MySqlResult::get_number(const std::string& column_name) const {
  size_t column_index = get_column_index(column_name);
  if (row_[column_index] == nullptr) return 0;

  try {
    return boost::lexical_cast<int64_t>(row_[column_index]);
  } catch (boost::bad_lexical_cast&) { return 0; }
}

std::string MySqlResult::get_string(const std::string& column_name) const {
  size_t column_index = get_column_index(column_name);
  if (row_[column_index] == nullptr) return std::string();

  return std::string(row_[column_index]);
}

const char* MySqlResult::get_stream(const std::string& column_name,
                                   unsigned long& size) const {
  size_t column_index = get_column_index(column_name);
  if (row_[column_index] == nullptr) {
    size = 0;
    return nullptr;
  }

  size = mysql_fetch_lengths(static_cast<MYSQL_RES*>(handle_))[column_index];
  return row_[column_index];
}

bool MySqlResult::next() {
  row_ = mysql_fetch_row(static_cast<MYSQL_RES*>(handle_));
  return row_ != nullptr;
}
}  // namespace FW::Database
