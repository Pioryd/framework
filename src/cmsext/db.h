#ifndef FW_CMSEXT_DB_H
#define FW_CMSEXT_DB_H

#include <cmsext/declarations.h>

namespace cmsext::db {
// Type {"mysql", "sqlite"}
bool connect(const cms::string& type, const cms::string& host,
             const cms::string& user, const cms::string& password,
             const cms::string& database_name, const cms::string& port,
             const cms::string& socket);

bool begin_transaction(int32_t sql_id);
bool commit_transaction(int32_t sql_id);
bool rollback_transaction(int32_t sql_id);

cms::string escape_number(int32_t sql_id, int64_t number);
cms::string escape_string(int32_t sql_id, const cms::string& string);
cms::string escape_blob(int32_t sql_id, const cms::string& blob);

bool optimize_tables(int32_t sql_id);
bool trigger_exists(int32_t sql_id, const cms::string& trigger);
bool table_exists(int32_t sql_id, const cms::string& table);
bool database_exists(int32_t sql_id);

cms::string get_sql_client_version(int32_t sql_id);
int64_t get_last_insert_id(int32_t sql_id);

bool execute_sync(int32_t sql_id, const cms::string& query);

cms::string get_string(int32_t sql_id, const cms::string& column);
int64_t get_number(int32_t sql_id, const cms::string& column);
}  // namespace cmsext::db
#endif  // #ifndef FW_CMSEXT_DB_H
