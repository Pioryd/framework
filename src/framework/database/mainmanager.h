#ifndef FW_DATABASE_MAINMANAGER_H
#define FW_DATABASE_MAINMANAGER_H

#include "declarations.h"

#include "sqlmanager.h"

namespace FW::Database {
enum class SqlType { MySql, Sqlite };

class Query {
 public:
  Query(std::string query, bool _multiValues = false);
  Query(std::string query, std::vector<std::string>& addidtionalValues);
  virtual ~Query() = default;

  bool addRow(const std::string& row);

  // Return complete query
  const std::string& toString() const;

 public:
  std::string completeQuery_;
  bool allowAddRows_;
  std::function<void(Result_sptr, bool)> callback_;
  bool store_;
};

class MainManager : protected SqlManager {
 public:
  MainManager(SqlType sqlType);
  virtual ~MainManager() = default;

  MainManager(const MainManager&) = delete;
  MainManager& operator=(const MainManager&) = delete;

  //
  // Override SqlManager
  //
 public:
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

  //
  // MainManager methods
  //
 public:
  void executeAsync(const Query& query);
  // Throw exeption on fail to execute query
  Result_sptr executeSync(const Query& query);

 protected:
  void internalExecute(const Query& query);

 protected:
  SqlManager_uptr sqlManager_;
};
}  // namespace FW::Database
#endif  // FW_DATABASE_MAINMANAGER_H
