#include "sqlitemanager.h"

namespace FW::Database {
SqliteManager::SqliteManager() : SqlManager() {}

SqliteManager::~SqliteManager() {}

bool SqliteManager::connect(const SqlConfig& sqlConfig) { return false; }

Result_sptr SqliteManager::execute(const std::string& query) {
  return Result_sptr();
}

bool SqliteManager::beginTransaction() { return false; }

bool SqliteManager::commitTransaction() { return false; }

bool SqliteManager::rollbackTransaction() { return false; }

std::string SqliteManager::escapeNumber(int64_t number) const { return ""; }

std::string SqliteManager::escapeString(const std::string& string) const {
  return "";
}

std::string SqliteManager::escapeBlob(const char* c_string,
                                      uint32_t length) const {
  return "";
}

bool SqliteManager::optimizeTables() { return false; }

bool SqliteManager::triggerExists(std::string trigger) { return false; }

bool SqliteManager::tableExists(std::string table) { return false; }

bool SqliteManager::databaseExists() { return false; }

std::string SqliteManager::getSqlClientVersion() const { return ""; }

int64_t SqliteManager::getLastInsertId() const { return 0; }

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
