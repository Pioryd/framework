#include "db.h"

#include <framework/database/mainmanager.h>
#include <framework/global.h>

#include <framework/pymodule/manager.h>
#include <pybind11/embed.h>

namespace cmsext::db {
bool connect(const cms::string& type, const cms::string& host,
             const cms::string& user, const cms::string& password,
             const cms::string& database_name, const cms::string& port,
             const cms::string& socket) {
  FW::Database::SqlConfig config;
  if (type == "mysql")
    config.type = FW::Database::SqlConfig::SqlType::MySql;
  else if (type == "sqlite")
    config.type = FW::Database::SqlConfig::SqlType::MySql;
  config.host = host;
  config.user = user;
  config.password = password;
  config.databaseName = database_name;
  config.port = std::stoi((std::string)port);
  config.socket = socket;

  return FW::G::db_manager.connect(config);
}

bool begin_transaction(int32_t sql_id) {
  return FW::G::db_manager.beginTransaction(sql_id);
}

bool commit_transaction(int32_t sql_id) {
  return FW::G::db_manager.commitTransaction(sql_id);
}

bool rollback_transaction(int32_t sql_id) {
  return FW::G::db_manager.rollbackTransaction(sql_id);
}

cms::string escape_number(int32_t sql_id, int64_t number) {
  return FW::G::db_manager.escapeNumber(sql_id, number);
}

cms::string escape_string(int32_t sql_id, const cms::string& string) {
  return FW::G::db_manager.escapeString(sql_id, string);
}

cms::string escape_blob(int32_t sql_id, const cms::string& blob) {
  return FW::G::db_manager.escapeBlob(sql_id, ((std::string)blob).c_str(),
                                      ((std::string)blob).size());
}

bool optimize_tables(int32_t sql_id) {
  return FW::G::db_manager.optimizeTables(sql_id);
}

bool trigger_exists(int32_t sql_id, const cms::string& trigger) {
  return FW::G::db_manager.triggerExists(sql_id, trigger);
}

bool table_exists(int32_t sql_id, const cms::string& table) {
  return FW::G::db_manager.tableExists(sql_id, table);
}

bool database_exists(int32_t sql_id) {
  return FW::G::db_manager.databaseExists(sql_id);
}

cms::string get_sql_client_version(int32_t sql_id) {
  return FW::G::db_manager.getSqlClientVersion(sql_id);
}

int64_t get_last_insert_id(int32_t sql_id) {
  return FW::G::db_manager.getLastInsertId(sql_id);
}

bool execute_sync(int32_t sql_id, const cms::string& query) {
  return FW::G::db_manager.executeSync(sql_id, (std::string)query);
}

cms::string get_string(int32_t sql_id, const cms::string& column) {
  return FW::G::db_manager.get_current_result(sql_id)->getString(column);
}

int64_t get_number(int32_t sql_id, const cms::string& column) {
  return FW::G::db_manager.get_current_result(sql_id)->getNumber(column);
}
}  // namespace cmsext::db

PYBIND11_EMBEDDED_MODULE(cmsext_db, m) {
  m.def("connect", cmsext::db::connect),  //
      m.def("begin_transaction", cmsext::db::begin_transaction),
      m.def("commit_transaction", cmsext::db::commit_transaction),
      m.def("rollback_transaction", cmsext::db::rollback_transaction),
      m.def("escape_number", cmsext::db::escape_number),
      m.def("escape_string", cmsext::db::escape_string),
      m.def("escape_blob", cmsext::db::escape_blob),
      m.def("optimize_tables", cmsext::db::optimize_tables),
      m.def("trigger_exists", cmsext::db::trigger_exists),
      m.def("table_exists", cmsext::db::table_exists),
      m.def("database_exists", cmsext::db::database_exists),
      m.def("get_sql_client_version", cmsext::db::get_sql_client_version),
      m.def("get_last_insert_id", cmsext::db::get_last_insert_id),
      m.def("execute_sync", cmsext::db::execute_sync),
      m.def("get_string", cmsext::db::get_string),
      m.def("get_number", cmsext::db::get_number);
}