#include "mainmanager.h"

#include "mysqlmanager.h"
#include "sqlitemanager.h"

namespace FW::Database {
Query::Query(std::string query, bool fullQuery)
    : completeQuery_{query}, allowAddRows_(fullQuery), store_{false} {}

Query::Query(std::string query, std::vector<std::string>& addidtionalRows)
    : completeQuery_{query}, allowAddRows_(true), store_{false} {}

bool Query::addRow(const std::string& row) { return false; }

const std::string& Query::toString() const { return ""; }

MainManager::MainManager(SqlType sqlType) : SqlManager() {}

bool MainManager::connect(const SqlConfig& sqlConfig) { return false; }

Result_sptr MainManager::execute(const std::string& query) {
  return Result_sptr();
}

bool MainManager::beginTransaction() { return false; }

bool MainManager::commitTransaction() { return false; }

bool MainManager::rollbackTransaction() { return false; }

std::string MainManager::escapeNumber(int64_t number) const { return ""; }

std::string MainManager::escapeString(const std::string& string) const {
  return "";
}

std::string MainManager::escapeBlob(const char* c_string,
                                    uint32_t length) const {
  return "";
}

bool MainManager::optimizeTables() { return false; }

bool MainManager::triggerExists(std::string trigger) { return false; }
bool MainManager::tableExists(std::string table) { return false; }

bool MainManager::databaseExists() { return false; }

std::string MainManager::getSqlClientVersion() const { return ""; }

int64_t MainManager::getLastInsertId() const { return 0; }

void MainManager::executeAsync(const Query& query) {}

Result_sptr MainManager::executeSync(const Query& query) {
  return Result_sptr();
}

void MainManager::internalExecute(const Query& query) {}
}  // namespace FW::Database