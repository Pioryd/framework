#include "mainmanager.h"

#include "../core/application.h"
#include "../global.h"
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

MainManager::MainManager() {}

void MainManager::terminate() {
  for (auto& db : db_vec_) db.event_manager->terminate();
}

void MainManager::join() {
  for (auto& db : db_vec_) db.event_manager->join();
}

bool MainManager::connect(const SqlConfig& sqlConfig) {
  if (sqlConfig.type == SqlConfig::SqlType::MySql)
    db_vec_.push_back(
        {nullptr,
         std::make_unique<FW::Thread::EventManager>(sqlConfig.databaseName),
         std::make_unique<MySqlManager>()});
  else if (sqlConfig.type == SqlConfig::SqlType::Sqlite)
    db_vec_.push_back(
        {nullptr,
         std::make_unique<FW::Thread::EventManager>(sqlConfig.databaseName),
         std::make_unique<SqliteManager>()});

  int32_t sql_id = db_vec_.size() - 1;
  if (!db_vec_[sql_id].sql->connect(sqlConfig)) {
    db_vec_.erase(db_vec_.end() - 1);
    return -1;
  }

  db_vec_[sql_id].event_manager->start();

  FW::G::Application->signals.onJoin.connect(
      "MainManager::join, this", std::bind(&MainManager::join, this));
  return sql_id;
}

Result_sptr MainManager::execute(int32_t sql_id, const std::string& query) {
  return db_vec_[sql_id].sql->execute(query);
}

bool MainManager::beginTransaction(int32_t sql_id) {
  return db_vec_[sql_id].sql->beginTransaction();
}

bool MainManager::commitTransaction(int32_t sql_id) {
  return db_vec_[sql_id].sql->commitTransaction();
}

bool MainManager::rollbackTransaction(int32_t sql_id) {
  return db_vec_[sql_id].sql->rollbackTransaction();
}

std::string MainManager::escapeNumber(int32_t sql_id, int64_t number) const {
  return db_vec_[sql_id].sql->escapeNumber(number);
}

std::string MainManager::escapeString(int32_t sql_id,
                                      const std::string& string) const {
  return db_vec_[sql_id].sql->escapeString(string);
}

std::string MainManager::escapeBlob(int32_t sql_id, const char* c_string,
                                    uint32_t length) const {
  return db_vec_[sql_id].sql->escapeBlob(c_string, length);
}

bool MainManager::optimizeTables(int32_t sql_id) {
  return db_vec_[sql_id].sql->optimizeTables();
}

bool MainManager::triggerExists(int32_t sql_id, std::string trigger) {
  return db_vec_[sql_id].sql->triggerExists(trigger);
}
bool MainManager::tableExists(int32_t sql_id, std::string table) {
  return db_vec_[sql_id].sql->tableExists(table);
}

bool MainManager::databaseExists(int32_t sql_id) {
  return db_vec_[sql_id].sql->databaseExists();
}

std::string MainManager::getSqlClientVersion(int32_t sql_id) const {
  return db_vec_[sql_id].sql->getSqlClientVersion();
}

int64_t MainManager::getLastInsertId(int32_t sql_id) const {
  return db_vec_[sql_id].sql->getLastInsertId();
}

void MainManager::executeAsync(int32_t sql_id, const Query& query) {
  db_vec_[sql_id].event_manager->addAsyncEvent(
      std::bind(static_cast<void (MainManager::*)(int32_t, const Query&)>(
                    &MainManager::internalExecute),
                this, sql_id, query));
}

bool MainManager::executeSync(int32_t sql_id, const Query& query) {
  db_vec_[sql_id].current_result =
      nullptr;  // TODO - Check if it's really needed.
  db_vec_[sql_id].current_result = execute(sql_id, query.toString());
  return (db_vec_[sql_id].current_result != nullptr);
}

Result_sptr MainManager::get_current_result(int32_t sql_id) {
  return db_vec_[sql_id].current_result;
}

void MainManager::internalExecute(int32_t sql_id, const Query& query) {
  bool success = true;
  Result_sptr result;
  try {
    result = execute(sql_id, query.toString());
  } catch (...) { success = false; }

  if (query.callback_) {
    db_vec_[sql_id].event_manager->addAsyncEvent(
        std::bind(query.callback_, result, success));
  }
}
}  // namespace FW::Database