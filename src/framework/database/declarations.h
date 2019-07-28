#ifndef FW_DATABASE_DECLARATIONS_H
#define FW_DATABASE_DECLARATIONS_H

#include "../pch.h"

namespace FW::Database {
class Query;
class Result;
class MySqlResult;
class MariadbResult;
class SqliteResult;
class PostgreSqlResult;
class SqlManager;
class MySqlManager;
class MariadbManager;
class SqliteManager;
class PostgreSqlManager;
class MainManager;

using Result_sptr = std::shared_ptr<Result>;
using SqlManager_uptr = std::unique_ptr<SqlManager>;
using MainManager_uptr = std::unique_ptr<MainManager>;
}  // namespace FW::Database
#endif  // #ifndef FW_DATABASE_DECLARATIONS_H