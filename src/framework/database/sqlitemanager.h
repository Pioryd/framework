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

  bool connect(const SqlConfig& sqlConfig) override;

  Result_sptr execute(const std::string& query) override;

  bool beginTransaction() override;
  bool commitTransaction() override;
  bool rollbackTransaction() override;

  std::string escapeNumber(int64_t number) const override;
  std::string escapeString(const std::string& string) const override;
  std::string escapeBlob(const char* blob, uint32_t length) const override;

  bool optimizeTables() override;
  bool triggerExists(std::string trigger) override;
  bool tableExists(std::string table) override;
  bool databaseExists() override;

  std::string getSqlClientVersion() const override;
  int64_t getLastInsertId() const override;

 protected:
  sqlite3* handle_ = nullptr;
};

class SqliteResult : public Result {
 public:
  SqliteResult(void* handle);
  virtual ~SqliteResult();

  SqliteResult(const Result&) = delete;
  SqliteResult& operator=(const Result&) = delete;

  int64_t getNumber(const std::string& columnName) const override;
  std::string getString(const std::string& columnName) const override;
  const char* getStream(const std::string& columnName,
                        unsigned long& size) const override;

 public:
  bool next() override;
};
}  // namespace FW::Database
#endif  // #ifndef FW_DATABASE_SQLITEMANAGER_H
