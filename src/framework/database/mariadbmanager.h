#ifndef FW_DATABASE_MARIADBMANAGER_H
#define FW_DATABASE_MARIADBMANAGER_H

#include "declarations.h"

#include "mysqlmanager.h"

namespace FW::Database {
class MariadbManager : public MySqlManager {
  MariadbManager();
  virtual ~MariadbManager() = default;

  MariadbManager(const MariadbManager&) = delete;
  MariadbManager& operator=(const MariadbManager&) = delete;
};
}  // namespace FW::Database
#endif  // #ifndef FW_DATABASE_MARIADBMANAGER_H
