#include "mainmanager.h"

#include "mysqlmanager.h"
#include "sqlitemanager.h"

namespace FW::Database {
Query::Query(std::string query, bool fullQuery)
    : completeQuery_{query}, allowAddRows_(fullQuery), store_{false} {}

Query::Query(std::string query, std::vector<std::string>& addidtionalRows)
    : completeQuery_{query}, allowAddRows_(true), store_{false} {
  for (const auto row : addidtionalRows) addRow(row);
}

bool Query::addRow(const std::string& row) {
  if (completeQuery_.empty()) return false;

  if (completeQuery_.back() == ')') completeQuery_.push_back(',');

  completeQuery_.push_back('(');
  completeQuery_.append(row);
  completeQuery_.push_back(')');

  return true;
}

const std::string& Query::toString() const { return completeQuery_; }

MainManager::MainManager(SqlType sqlType)
    : EventManager("MainManager"), SqlManager() {
  if (sqlType == SqlType::MySql) {
    sqlManager_ = std::make_unique<MySqlManager>();
  } else if (sqlType == SqlType::Sqlite) {
    sqlManager_ = std::make_unique<SqliteManager>();
  }
}

bool MainManager::connect(const SqlConfig& sqlConfig) {
  return sqlManager_->connect(sqlConfig);
}

Result_sptr MainManager::execute(const std::string& query) {
  return sqlManager_->execute(query);
}

bool MainManager::beginTransaction() { return sqlManager_->beginTransaction(); }

bool MainManager::commitTransaction() {
  return sqlManager_->commitTransaction();
}

bool MainManager::rollbackTransaction() {
  return sqlManager_->rollbackTransaction();
}

std::string MainManager::escapeNumber(int64_t number) const {
  return sqlManager_->escapeNumber(number);
}

std::string MainManager::escapeString(const std::string& string) const {
  return sqlManager_->escapeString(string);
}

std::string MainManager::escapeBlob(const char* c_string,
                                    uint32_t length) const {
  return sqlManager_->escapeBlob(c_string, length);
}

bool MainManager::optimizeTables() { return sqlManager_->optimizeTables(); }

bool MainManager::triggerExists(std::string trigger) {
  return sqlManager_->triggerExists(trigger);
}
bool MainManager::tableExists(std::string table) {
  return sqlManager_->tableExists(table);
}

bool MainManager::databaseExists() { return sqlManager_->databaseExists(); }

std::string MainManager::getSqlClientVersion() const {
  return sqlManager_->getSqlClientVersion();
}

int64_t MainManager::getLastInsertId() const {
  return sqlManager_->getLastInsertId();
}

void MainManager::executeAsync(const Query& query) {
  addAsyncEvent(std::bind(static_cast<void (MainManager::*)(const Query&)>(
                              &MainManager::internalExecute),
                          this, query));
}

Result_sptr MainManager::executeSync(const Query& query) {
  return execute(query.toString());
}

void MainManager::internalExecute(const Query& query) {
  bool success = true;
  Result_sptr result;
  try {
    result = execute(query.toString());
  } catch (...) { success = false; }

  if (query.callback_) {
    addAsyncEvent(std::bind(query.callback_, result, success));
  }
}
}  // namespace FW::Database