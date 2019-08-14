#include "sqlitemanager.h"

#include "../core/application.h"
#include "../core/logger.h"
#include "../global.h"

namespace FW::Database {
SqliteManager::SqliteManager() : SqlManager() {}

SqliteManager::~SqliteManager() {
  if (transaction_state_ == TransactionState::BEGIN) rollback_transaction();
  if (handle_ != nullptr) sqlite3_close(handle_);
}

bool SqliteManager::connect(const SqlConfig& sql_config) {
  SqlManager::connect(sql_config);

  std::filesystem::path path(G::Application->get_working_directory());
  path.append(sql_config.database_name.c_str());

  if (!std::filesystem::exists(path)) {
    G::Logger.error("Sqlite database[" + sql_config.database_name +
                    "] does not exist");
    return false;
  }

  int rc = sqlite3_open(path.string().c_str(), &handle_);
  if (rc != SQLITE_OK) {
    G::Logger.error("Can't open Sqlite database[" + sql_config.database_name +
                    "]. Sqlite error: " + std::string(sqlite3_errmsg(handle_)));
    sqlite3_close(handle_);
    handle_ = nullptr;
  }

  return true;
}

Result_sptr SqliteManager::execute(const std::string& query) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);

  if (!handle_) return nullptr;

  FW_DEBUG_INSTRUCTIONS(
      G::Logger.debug("query(" + sql_config_.database_name + "): " + query);)

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(handle_, query.c_str(), query.length(), &stmt, NULL) !=
      SQLITE_OK) {
    sqlite3_finalize(stmt);
    G::Logger.error("Sqlite error(" + std::string(sqlite3_errmsg(handle_)) +
                    "). Failed to execute query[" + sql_config_.database_name +
                    "]: " + query);
    throw std::runtime_error("Execute query failed.");
  }

  Result_sptr result;
  try {
    result = std::make_shared<SqliteResult>(stmt);
  } catch (std::exception e) {
    G::Logger.error(e.what());
    return nullptr;
  }

  if (!result->next()) return nullptr;

  return result;
}

bool SqliteManager::begin_transaction() {
  if (transaction_state_ != TransactionState::NONE) return false;
  if (!execute("BEGIN")) return false;

  mutex_.lock();
  transaction_state_ = TransactionState::BEGIN;
  return true;
}

bool SqliteManager::commit_transaction() {
  if (transaction_state_ != TransactionState::BEGIN) return false;
  if (!execute("COMMIT")) {
    transaction_state_ = TransactionState::FAIL;
    return false;
  }

  transaction_state_ = TransactionState::NONE;
  mutex_.unlock();
  return true;
}

bool SqliteManager::rollback_transaction() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);

  if (transaction_state_ == TransactionState::NONE) return false;
  if (!execute("ROLLBACK")) {
    transaction_state_ = TransactionState::FAIL;
    return false;
  }

  transaction_state_ = TransactionState::NONE;
  mutex_.unlock();
  return true;
}

std::string SqliteManager::escape_number(int64_t number) const {
  return escape_string(std::to_string(number));
}

std::string SqliteManager::escape_string(const std::string& string) const {
  if (string.empty()) return "''";

  // NOTE: I'm not sure that's correct.
  // [string * 2] in case when all charater will escaped
  // [+2] for two quotes
  // [+1] for null-terminate string
  auto max_size_of_c_string = (string.length() * 2) + 2 + 1;
  char* output = new char[max_size_of_c_string];

  // TODO
  // escape other chars

  // %Q - care only for quotes
  sqlite3_snprintf(max_size_of_c_string, output, "%Q", string.c_str());

  std::string escaped(output);
  delete[] output;

  return escaped;
}

std::string SqliteManager::escape_blob(const char* c_string,
                                      uint32_t length) const {
  if (length == 0) return "''";

  std::string return_string = "x'";

  // [2] 2 characters for every 1 element in the array
  // [+1] for null-terminate string
  char hex[2 + 1];

  for (uint32_t i = 0; i < length; i++) {
    sprintf(hex, "%02x", ((uint8_t)c_string[i]));
    return_string += hex;
  }

  return_string += "'";

  return return_string;
}

bool SqliteManager::optimize_tables() {
  try {
    static_cast<void>(execute("VACUUM;"));
  } catch (std::exception e) {
    G::Logger.error(e.what());
    return false;
  }
  return true;
}

bool SqliteManager::trigger_exists(std::string trigger) {
  std::ostringstream query;
  query << "SELECT `name` FROM `sqlite_master` WHERE `type` = 'trigger' AND "
        << "`name` = " << escape_string(trigger) << ";";
  try {
    return (execute(query.str()) != nullptr);
  } catch (std::exception e) { G::Logger.error(e.what()); }
  return false;
}
bool SqliteManager::table_exists(std::string table) {
  std::ostringstream query;
  query << "SELECT `name` FROM `sqlite_master` WHERE `type` = 'table' AND "
        << "`name` = " << escape_string(table) << ";";

  try {
    return (execute(query.str()) != nullptr);
  } catch (std::exception e) { G::Logger.error(e.what()); }
  return false;
}

bool SqliteManager::database_exists() { return (handle_ != nullptr); }

std::string SqliteManager::get_sql_client_version() const {
  return SQLITE_VERSION;
}

int64_t SqliteManager::get_last_insert_id() const {
  return static_cast<uint64_t>(sqlite3_last_insert_rowid(handle_));
}

SqliteResult::SqliteResult(void* sqliteResult) : Result(sqliteResult) {
  if (!sqliteResult)
    throw std::runtime_error("Sqlite handle to result cannot be nullptr.");

  int32_t fields_count =
      sqlite3_column_count(static_cast<sqlite3_stmt*>(handle_));
  for (int32_t i = 0; i < fields_count; i++)
    field_names_[sqlite3_column_name(static_cast<sqlite3_stmt*>(handle_), i)] =
        i;
}

SqliteResult::~SqliteResult() {
  if (handle_) sqlite3_finalize(static_cast<sqlite3_stmt*>(handle_));
}
int64_t SqliteResult::get_number(const std::string& column_name) const {
  size_t column_index = get_column_index(column_name);
  return sqlite3_column_int64(static_cast<sqlite3_stmt*>(handle_), column_index);
}

std::string SqliteResult::get_string(const std::string& column_name) const {
  size_t column_index = get_column_index(column_name);
  std::string value = reinterpret_cast<const char*>(
      sqlite3_column_text(static_cast<sqlite3_stmt*>(handle_), column_index));
  return value;
}

const char* SqliteResult::get_stream(const std::string& column_name,
                                    unsigned long& size) const {
  size_t column_index = get_column_index(column_name);
  const char* value = static_cast<const char*>(
      sqlite3_column_blob(static_cast<sqlite3_stmt*>(handle_), column_index));
  size = sqlite3_column_bytes(static_cast<sqlite3_stmt*>(handle_), column_index);
  return value;
}

bool SqliteResult::next() {
  return (sqlite3_step(static_cast<sqlite3_stmt*>(handle_)) == SQLITE_ROW);
}
}  // namespace FW::Database
