#ifndef FW_DATABASE_SQLITEMANAGER_H
#define FW_DATABASE_SQLITEMANAGER_H

#include "declarations.h"

#include "sqlmanager.h"

#include <sqlite3.h>

namespace FW::Database {
class SqliteManager : public SqlManager {
 public:
  SqliteManager();
  virtual ~SqliteManager();

  SqliteManager(const SqliteManager&) = delete;
  SqliteManager& operator=(const SqliteManager&) = delete;

  bool connect(const SqlConfig& sql_config) override;

  Result_sptr execute(const std::string& query) override;

  bool begin_transaction() override;
  bool commit_transaction() override;
  bool rollback_transaction() override;

  std::string escape_number(int64_t number) const override;
  std::string escape_string(const std::string& string) const override;
  std::string escape_blob(const char* blob, uint32_t length) const override;

  bool optimize_tables() override;
  bool trigger_exists(std::string trigger) override;
  bool table_exists(std::string table) override;
  bool database_exists() override;

  std::string get_sql_client_version() const override;
  int64_t get_last_insert_id() const override;

 protected:
  sqlite3* handle_ = nullptr;
};

class SqliteResult : public Result {
 public:
  SqliteResult(void* handle);
  virtual ~SqliteResult();

  SqliteResult(const Result&) = delete;
  SqliteResult& operator=(const Result&) = delete;

  int64_t get_number(const std::string& column_name) const override;
  std::string get_string(const std::string& column_name) const override;
  const char* get_stream(const std::string& column_name,
                        unsigned long& size) const override;

 public:
  bool next() override;
};
}  // namespace FW::Database
#endif  // #ifndef FW_DATABASE_SQLITEMANAGER_H
