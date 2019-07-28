#include "sqlitemanager.h"

#include "../core/application.h"
#include "../core/logger.h"
#include "../global.h"

namespace FW::Database {
SqliteManager::SqliteManager() : SqlManager() {}

SqliteManager::~SqliteManager() {
  if (transactionState_ == TransactionState::BEGIN) rollbackTransaction();
  if (handle_ != nullptr) sqlite3_close(handle_);
}

bool SqliteManager::connect(const SqlConfig& sqlConfig) {
  SqlManager::connect(sqlConfig);

  std::filesystem::path path(G::Application->getWorkingDirectory());
  path.append(sqlConfig.databaseName.c_str());

  if (!std::filesystem::exists(path)) {
    G::Logger.error("Sqlite database[" + sqlConfig.databaseName +
                    "] does not exist");
    return false;
  }

  int rc = sqlite3_open(path.string().c_str(), &handle_);
  if (rc != SQLITE_OK) {
    G::Logger.error("Can't open Sqlite database[" + sqlConfig.databaseName +
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
      G::Logger.debug("query(" + sqlConfig_.databaseName + "): " + query);)

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(handle_, query.c_str(), query.length(), &stmt, NULL) !=
      SQLITE_OK) {
    sqlite3_finalize(stmt);
    G::Logger.error("Sqlite error(" + std::string(sqlite3_errmsg(handle_)) +
                    "). Failed to execute query[" + sqlConfig_.databaseName +
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

bool SqliteManager::beginTransaction() {
  if (transactionState_ != TransactionState::NONE) return false;
  if (!execute("BEGIN")) return false;

  mutex_.lock();
  transactionState_ = TransactionState::BEGIN;
  return true;
}

bool SqliteManager::commitTransaction() {
  if (transactionState_ != TransactionState::BEGIN) return false;
  if (!execute("COMMIT")) {
    transactionState_ = TransactionState::FAIL;
    return false;
  }

  transactionState_ = TransactionState::NONE;
  mutex_.unlock();
  return true;
}

bool SqliteManager::rollbackTransaction() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);

  if (transactionState_ == TransactionState::NONE) return false;
  if (!execute("ROLLBACK")) {
    transactionState_ = TransactionState::FAIL;
    return false;
  }

  transactionState_ = TransactionState::NONE;
  mutex_.unlock();
  return true;
}

std::string SqliteManager::escapeNumber(int64_t number) const {
  return escapeString(std::to_string(number));
}

std::string SqliteManager::escapeString(const std::string& string) const {
  if (string.empty()) return "''";

  // NOTE: I'm not sure that's correct.
  // [string * 2] in case when all charater will escaped
  // [+2] for two quotes
  // [+1] for null-terminate string
  auto maxSizeOfC_string = (string.length() * 2) + 2 + 1;
  char* output = new char[maxSizeOfC_string];

  // TODO
  // escape other chars

  // %Q - care only for quotes
  sqlite3_snprintf(maxSizeOfC_string, output, "%Q", string.c_str());

  std::string escaped(output);
  delete[] output;

  return escaped;
}

std::string SqliteManager::escapeBlob(const char* c_string,
                                      uint32_t length) const {
  if (length == 0) return "''";

  std::string returnString = "x'";

  // [2] 2 characters for every 1 element in the array
  // [+1] for null-terminate string
  char hex[2 + 1];

  for (uint32_t i = 0; i < length; i++) {
    sprintf(hex, "%02x", ((uint8_t)c_string[i]));
    returnString += hex;
  }

  returnString += "'";

  return returnString;
}

bool SqliteManager::optimizeTables() {
  try {
    static_cast<void>(execute("VACUUM;"));
  } catch (std::exception e) {
    G::Logger.error(e.what());
    return false;
  }
  return true;
}

bool SqliteManager::triggerExists(std::string trigger) {
  std::ostringstream query;
  query << "SELECT `name` FROM `sqlite_master` WHERE `type` = 'trigger' AND "
        << "`name` = " << escapeString(trigger) << ";";
  try {
    return (execute(query.str()) != nullptr);
  } catch (std::exception e) { G::Logger.error(e.what()); }
  return false;
}
bool SqliteManager::tableExists(std::string table) {
  std::ostringstream query;
  query << "SELECT `name` FROM `sqlite_master` WHERE `type` = 'table' AND "
        << "`name` = " << escapeString(table) << ";";

  try {
    return (execute(query.str()) != nullptr);
  } catch (std::exception e) { G::Logger.error(e.what()); }
  return false;
}

bool SqliteManager::databaseExists() { return (handle_ != nullptr); }

std::string SqliteManager::getSqlClientVersion() const {
  return SQLITE_VERSION;
}

int64_t SqliteManager::getLastInsertId() const {
  return static_cast<uint64_t>(sqlite3_last_insert_rowid(handle_));
}

SqliteResult::SqliteResult(void* sqliteResult) : Result(sqliteResult) {}

SqliteResult::~SqliteResult() {}
int64_t SqliteResult::getNumber(const std::string& columnName) const {
  return 0;
}

std::string SqliteResult::getString(const std::string& columnName) const {
  return "";
}

const char* SqliteResult::getStream(const std::string& columnName,
                                    unsigned long& size) const {
  return "";
}

bool SqliteResult::next() { return false; }
}  // namespace FW::Database
