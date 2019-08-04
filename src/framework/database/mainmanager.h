#ifndef FW_DATABASE_MAINMANAGER_H
#define FW_DATABASE_MAINMANAGER_H

#include "declarations.h"

#include "sqlmanager.h"

#include "../thread/eventmanager.h"

namespace FW::Database {
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

class MainManager {
  struct DB {
    Result_sptr current_result;
    Thread::EventManager_uptr event_manager;
    SqlManager_uptr sql;
  };

 public:
  MainManager();
  virtual ~MainManager() = default;

  MainManager(const MainManager&) = delete;
  MainManager& operator=(const MainManager&) = delete;

  void terminate();
  void join();

 public:
  int32_t connect(const SqlConfig& sqlConfig);

  Result_sptr execute(int32_t sql_id, const std::string& query);

  bool beginTransaction(int32_t sql_id);
  bool commitTransaction(int32_t sql_id);
  bool rollbackTransaction(int32_t sql_id);

  std::string escapeNumber(int32_t sql_id, int64_t number) const;
  std::string escapeString(int32_t sql_id, const std::string& string) const;
  std::string escapeBlob(int32_t sql_id, const char* blob,
                         uint32_t length) const;

  bool optimizeTables(int32_t sql_id);
  bool triggerExists(int32_t sql_id, std::string trigger);
  bool tableExists(int32_t sql_id, std::string table);
  bool databaseExists(int32_t sql_id);

  std::string getSqlClientVersion(int32_t sql_id) const;
  int64_t getLastInsertId(int32_t sql_id) const;

 public:
  // TODO
  // Asyn need tests. Could be unsafe.
  void executeAsync(int32_t sql_id, const Query& query);

  // Throw exeption on fail.
  bool executeSync(int32_t sql_id, const Query& query);
  Result_sptr get_current_result(int32_t sql_id);

 protected:
  void internalExecute(int32_t sql_id, const Query& query);

 protected:
  std::vector<DB> db_vec_;
};
}  // namespace FW::Database
#endif  // FW_DATABASE_MAINMANAGER_H
