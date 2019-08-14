#include "mainmanager.h"

#include "../core/application.h"
#include "../global.h"
#include "mysqlmanager.h"
#include "sqlitemanager.h"

namespace FW::Database {
Query::Query(std::string query, bool fullQuery)
    : complete_query_{query}, allow_add_rows_(fullQuery), store_{false} {}

Query::Query(std::string query, std::vector<std::string>& addidtionalRows)
    : complete_query_{query}, allow_add_rows_(true), store_{false} {
  for (const auto row : addidtionalRows) add_row(row);
}

bool Query::add_row(const std::string& row) {
  if (complete_query_.empty()) return false;

  if (complete_query_.back() == ')') complete_query_.push_back(',');

  complete_query_.push_back('(');
  complete_query_.append(row);
  complete_query_.push_back(')');

  return true;
}

const std::string& Query::to_string() const { return complete_query_; }

MainManager::MainManager() {}

void MainManager::terminate() {
  for (auto& db : db_vec_) db.event_manager->terminate();
}

void MainManager::join() {
  for (auto& db : db_vec_) db.event_manager->join();
}

int32_t MainManager::connect(const SqlConfig& sql_config) {
  if (sql_config.type == SqlConfig::SqlType::MySql)
    db_vec_.push_back(
        {nullptr,
         std::make_unique<FW::Thread::EventManager>(sql_config.database_name),
         std::make_unique<MySqlManager>()});
  else if (sql_config.type == SqlConfig::SqlType::Sqlite)
    db_vec_.push_back(
        {nullptr,
         std::make_unique<FW::Thread::EventManager>(sql_config.database_name),
         std::make_unique<SqliteManager>()});

  int32_t sql_id = db_vec_.size() - 1;
  if (!db_vec_[sql_id].sql->connect(sql_config)) {
    db_vec_.erase(db_vec_.end() - 1);
    return -1;
  }

  db_vec_[sql_id].event_manager->start();

  FW::G::Application->signals.on_join.connect(
      "MainManager::join, this", std::bind(&MainManager::join, this));
  return sql_id;
}

Result_sptr MainManager::execute(int32_t sql_id, const std::string& query) {
  return db_vec_[sql_id].sql->execute(query);
}

bool MainManager::begin_transaction(int32_t sql_id) {
  return db_vec_[sql_id].sql->begin_transaction();
}

bool MainManager::commit_transaction(int32_t sql_id) {
  return db_vec_[sql_id].sql->commit_transaction();
}

bool MainManager::rollback_transaction(int32_t sql_id) {
  return db_vec_[sql_id].sql->rollback_transaction();
}

std::string MainManager::escape_number(int32_t sql_id, int64_t number) const {
  return db_vec_[sql_id].sql->escape_number(number);
}

std::string MainManager::escape_string(int32_t sql_id,
                                      const std::string& string) const {
  return db_vec_[sql_id].sql->escape_string(string);
}

std::string MainManager::escape_blob(int32_t sql_id, const char* c_string,
                                    uint32_t length) const {
  return db_vec_[sql_id].sql->escape_blob(c_string, length);
}

bool MainManager::optimize_tables(int32_t sql_id) {
  return db_vec_[sql_id].sql->optimize_tables();
}

bool MainManager::trigger_exists(int32_t sql_id, std::string trigger) {
  return db_vec_[sql_id].sql->trigger_exists(trigger);
}
bool MainManager::table_exists(int32_t sql_id, std::string table) {
  return db_vec_[sql_id].sql->table_exists(table);
}

bool MainManager::database_exists(int32_t sql_id) {
  return db_vec_[sql_id].sql->database_exists();
}

std::string MainManager::get_sql_client_version(int32_t sql_id) const {
  return db_vec_[sql_id].sql->get_sql_client_version();
}

int64_t MainManager::get_last_insert_id(int32_t sql_id) const {
  return db_vec_[sql_id].sql->get_last_insert_id();
}

void MainManager::executeAsync(int32_t sql_id, const Query& query) {
  db_vec_[sql_id].event_manager->add_async_event(
      std::bind(static_cast<void (MainManager::*)(int32_t, const Query&)>(
                    &MainManager::internal_execute),
                this, sql_id, query));
}

bool MainManager::executeSync(int32_t sql_id, const Query& query) {
  db_vec_[sql_id].current_result =
      nullptr;  // TODO - Check if it's really needed.
  db_vec_[sql_id].current_result = execute(sql_id, query.to_string());
  return (db_vec_[sql_id].current_result != nullptr);
}

Result_sptr MainManager::get_current_result(int32_t sql_id) {
  return db_vec_[sql_id].current_result;
}

void MainManager::internal_execute(int32_t sql_id, const Query& query) {
  bool success = true;
  Result_sptr result;
  try {
    result = execute(sql_id, query.to_string());
  } catch (...) { success = false; }

  if (query.callback_) {
    db_vec_[sql_id].event_manager->add_async_event(
        std::bind(query.callback_, result, success));
  }
}
}  // namespace FW::Database