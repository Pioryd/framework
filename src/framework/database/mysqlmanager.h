#ifndef FW_DATABASE_MYSQLMANAGER_H
#define FW_DATABASE_MYSQLMANAGER_H

#include "declarations.h"

#include "sqlmanager.h"

#include <mysql/mysql.h>

namespace FW::Database {
class MySqlManager : public SqlManager {
 private:
  static constexpr int32_t MYSQL_OK{0};
  static constexpr int32_t QUERY_ATTEMPTS = 3;

 public:
  MySqlManager();
  virtual ~MySqlManager();

  MySqlManager(const MySqlManager&) = delete;
  MySqlManager& operator=(const MySqlManager&) = delete;

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

 private:
  bool is_connection_error(uint32_t errorCode);
  bool internal_execute(const std::string& query);

 protected:
  MYSQL* handle_;
};

class MySqlResult : public Result {
 public:
  MySqlResult(void* handle);
  virtual ~MySqlResult();

  MySqlResult(const Result&) = delete;
  MySqlResult& operator=(const Result&) = delete;

  int64_t get_number(const std::string& column_name) const override;
  std::string get_string(const std::string& column_name) const override;
  const char* get_stream(const std::string& column_name,
                        unsigned long& size) const override;

  bool next() override;

 protected:
  MYSQL_ROW row_;
};
}  // namespace FW::Database
#endif  // #ifndef FW_DATABASE_MYSQLMANAGER_H
