#include "mysqlmanager.h"

namespace FW::Database {
MySqlManager::MySqlManager() : SqlManager() {}

MySqlManager::~MySqlManager() {}

bool MySqlManager::connect(const SqlConfig& sqlConfig) { return false; }

Result_sptr MySqlManager::execute(const std::string& query) {
  return Result_sptr();
}

bool MySqlManager::beginTransaction() { return false; }

bool MySqlManager::commitTransaction() { return false; }

bool MySqlManager::rollbackTransaction() { return false; }

std::string MySqlManager::escapeNumber(int64_t number) const { return ""; }

std::string MySqlManager::escapeString(const std::string& s) const {
  return "";
}

std::string MySqlManager::escapeBlob(const char* s, uint32_t length) const {
  return "";
}

bool MySqlManager::optimizeTables() { return false; }

bool MySqlManager::triggerExists(std::string trigger) { return false; }

bool MySqlManager::tableExists(std::string table) { return false; }

bool MySqlManager::databaseExists() { return false; }

std::string MySqlManager::getSqlClientVersion() const { return ""; }

int64_t MySqlManager::getLastInsertId() const { return 0; }

bool MySqlManager::isConnectionError(uint32_t errorCode) { return false; }

bool MySqlManager::internalExecute(const std::string& query) { return false; }
}  // namespace FW::Database
