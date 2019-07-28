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
}  // namespace FW::Database
