#include <cmsext/db.h>
#include <cmsext/log.h>
#include <cmsext/net.h>
#include <cmsext/thread.h>

using namespace cmsext::db;
using namespace cmsext::log;
using namespace cmsext::thread;

CMS_BEGIN(db_mod, "")
void db_test() {
  auto sql_id = connect("mysql", "127.0.0.1", "root", "", "test", "3306", "");

  cms::string query_2 = "SELECT `name` FROM `account` WHERE `id` = '1'";
  if (execute_sync(sql_id, query_2))
    info("Query:\n\t" + query_2 + "\nresult:\n\t" + get_string(sql_id, "name"));
  else
    info("Unable to execute query: " + query_2);
}
// Signals
void on_load() {}
void on_init() { db_test(); }
void on_terminate() {}
void on_reload() {}

CMS_END()
