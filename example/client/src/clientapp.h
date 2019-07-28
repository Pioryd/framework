#ifndef FW_CLIENT_APP_H
#define FW_CLIENT_APP_H

#include "declarations.h"

#include <framework/net/declarations.h>

namespace Client {
class ClientApp {
 public:
  enum class State { NONE, INITIALIZING, INITIALIZED, TERMINATED };

 public:
  struct Config {
    std::string appName = "MyServer";
    std::string sqlType = "sqlite";  // "mysql", "sqlite"
    std::string mysqlHost = "127.0.0.1";
    std::string mysqlUser = "root";
    std::string mysqlPassword = "";
    std::string mysqlName = "server.db";
    uint32_t mysqlPort = 3306;
    std::string mysqlSocket = "";

    std::string protocolAppHost = "127.0.0.1";
    uint16_t protocolAppPort = 7521;
  };

 public:
  ClientApp();

  void init(const std::vector<std::string>& argv, const std::string& title);

 protected:
  void onTerminate();
  void onJoin();

  void setup();

  void setupSignals();
  void setupRsa();
  void setupDatabase();
  void setupTcpListeners();

 public:
  Config config;

 protected:
  State state_;
  std::mutex mutex;
  std::condition_variable condition;

  FW::Net::TcpClient_ptr tcpClient;
};
}  // namespace Client
#endif  // #ifndef FW_CLIENT_APP_H
