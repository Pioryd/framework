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

  bool add_row(const std::string& row);

  // Return complete query
  const std::string& to_string() const;

 public:
  std::string complete_query_;
  bool allow_add_rows_;
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
  int32_t connect(const SqlConfig& sql_config);

  Result_sptr execute(int32_t sql_id, const std::string& query);

  bool begin_transaction(int32_t sql_id);
  bool commit_transaction(int32_t sql_id);
  bool rollback_transaction(int32_t sql_id);

  std::string escape_number(int32_t sql_id, int64_t number) const;
  std::string escape_string(int32_t sql_id, const std::string& string) const;
  std::string escape_blob(int32_t sql_id, const char* blob,
                         uint32_t length) const;

  bool optimize_tables(int32_t sql_id);
  bool trigger_exists(int32_t sql_id, std::string trigger);
  bool table_exists(int32_t sql_id, std::string table);
  bool database_exists(int32_t sql_id);

  std::string get_sql_client_version(int32_t sql_id) const;
  int64_t get_last_insert_id(int32_t sql_id) const;

 public:
  // TODO
  // Asyn need tests. Could be unsafe.
  void executeAsync(int32_t sql_id, const Query& query);

  // Throw exeption on fail.
  bool executeSync(int32_t sql_id, const Query& query);
  Result_sptr get_current_result(int32_t sql_id);

 protected:
  void internal_execute(int32_t sql_id, const Query& query);

 protected:
  std::vector<DB> db_vec_;
};
}  // namespace FW::Database
#endif  // FW_DATABASE_MAINMANAGER_H
