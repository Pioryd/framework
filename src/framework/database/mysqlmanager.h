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

 private:
  bool isConnectionError(uint32_t errorCode);
  bool internalExecute(const std::string& query);

 protected:
  MYSQL* handle_;
};

class MySqlResult : public Result {};
}  // namespace FW::Database
#endif  // #ifndef FW_DATABASE_MYSQLMANAGER_H
