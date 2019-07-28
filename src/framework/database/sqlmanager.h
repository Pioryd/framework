#ifndef FW_DATABASE_SQLMANAGER_H
#define FW_DATABASE_SQLMANAGER_H

#include "declarations.h"

#include <mutex>

namespace FW::Database {
enum class TransactionState { BEGIN, FAIL, NONE };

struct SqlConfig {
  std::string host;
  std::string user;
  std::string password;
  std::string databaseName;
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
  virtual bool connect(const SqlConfig& sqlConfig);

  // NOTE: The biggest positive number value is int64_t,
  // dont put uint64_t numbers, because it may cause loss of data
  virtual Result_sptr execute(const std::string& query) = 0;

  // Using only 3 belows functions to handle transaction is not safe,
  // because they unlock thread only if commitTransaction() or
  // rollbackTransaction() is success. In other ways thread will still locked.
  // Another possibility is forgot by user to commit/rollback.
  // In that case use class MainManager to safe handle transactions.
  virtual bool beginTransaction() = 0;
  virtual bool commitTransaction() = 0;
  virtual bool rollbackTransaction() = 0;

  virtual std::string escapeNumber(int64_t number) const = 0;
  virtual std::string escapeString(const std::string& string) const = 0;
  virtual std::string escapeBlob(const char* blob, uint32_t length) const = 0;

  virtual bool optimizeTables() = 0;
  virtual bool triggerExists(std::string trigger) = 0;
  virtual bool tableExists(std::string table) = 0;
  // Check if exist after conecconnectedcted to client.
  virtual bool databaseExists() = 0;

  virtual std::string getSqlClientVersion() const = 0;
  // If no rows have been inserted since the last query,
  // this function will return zero (0)
  virtual int64_t getLastInsertId() const = 0;

 public:
  Config config;

 protected:
  SqlConfig sqlConfig_;
  TransactionState transactionState_;
  std::recursive_mutex mutex_;
};

class Result {
 public:
  Result(void* handle);
  virtual ~Result() = default;

  Result(const Result&) = delete;
  Result& operator=(const Result&) = delete;

  virtual int64_t getNumber(const std::string& columnName) const = 0;
  virtual std::string getString(const std::string& columnName) const = 0;
  virtual const char* getStream(const std::string& columnName,
                                unsigned long& size) const = 0;

  virtual bool next() = 0;

 protected:
  size_t getColumnIndex(const std::string& columnName) const;

 protected:
  void* handle_;
  std::map<std::string, size_t> fieldNames_;
};
}  // namespace FW::Database
#endif  // #ifndef FW_DATABASE_SQLMANAGER_H
