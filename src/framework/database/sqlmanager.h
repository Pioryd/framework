#ifndef FW_DATABASE_SQLMANAGER_H
#define FW_DATABASE_SQLMANAGER_H

#include "declarations.h"

#include <mutex>

namespace FW::Database {
enum class TransactionState { BEGIN, FAIL, NONE };

struct SqlConfig {
  enum class SqlType { None, MySql, Sqlite };

  SqlType type{SqlType::None};
  std::string host;
  std::string user;
  std::string password;
  std::string database_name;
  uint32_t port{0};
  std::string socket;
};

class SqlManager {
 public:
  struct Config {
    bool debug_enabled = true;
  };

 public:
  SqlManager();
  virtual ~SqlManager() = default;

  SqlManager(const SqlManager&) = delete;
  SqlManager& operator=(const SqlManager&) = delete;

  // Must be called from main thread, becouse it run async thread
  virtual bool connect(const SqlConfig& sql_config);

  // NOTE: The biggest positive number value is int64_t,
  // dont put uint64_t numbers, because it may cause loss of data
  virtual Result_sptr execute(const std::string& query) = 0;

  // Using only 3 belows functions to handle transaction is not safe,
  // because they unlock thread only if commit_transaction() or
  // rollback_transaction() is success. In other ways thread will still locked.
  // Another possibility is forgot by user to commit/rollback.
  // In that case use class MainManager to safe handle transactions.
  virtual bool begin_transaction() = 0;
  virtual bool commit_transaction() = 0;
  virtual bool rollback_transaction() = 0;

  virtual std::string escape_number(int64_t number) const = 0;
  virtual std::string escape_string(const std::string& string) const = 0;
  virtual std::string escape_blob(const char* blob, uint32_t length) const = 0;

  virtual bool optimize_tables() = 0;
  virtual bool trigger_exists(std::string trigger) = 0;
  virtual bool table_exists(std::string table) = 0;
  // Check if exist after conecconnectedcted to client.
  virtual bool database_exists() = 0;

  virtual std::string get_sql_client_version() const = 0;
  // If no rows have been inserted since the last query,
  // this function will return zero (0)
  virtual int64_t get_last_insert_id() const = 0;

 public:
  Config config;

 protected:
  SqlConfig sql_config_;
  TransactionState transaction_state_;
  std::recursive_mutex mutex_;
};

class Result {
 public:
  Result(void* handle);
  virtual ~Result() = default;

  Result(const Result&) = delete;
  Result& operator=(const Result&) = delete;

  virtual int64_t get_number(const std::string& column_name) const = 0;
  virtual std::string get_string(const std::string& column_name) const = 0;
  virtual const char* get_stream(const std::string& column_name,
                                unsigned long& size) const = 0;

  virtual bool next() = 0;

 protected:
  size_t get_column_index(const std::string& column_name) const;

 protected:
  void* handle_;
  std::map<std::string, size_t> field_names_;
};
}  // namespace FW::Database
#endif  // #ifndef FW_DATABASE_SQLMANAGER_H
