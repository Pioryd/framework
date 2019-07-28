#ifndef FW_SERVER_APP_H
#define FW_SERVER_APP_H

#include "declarations.h"

#include <framework/net/declarations.h>

namespace Server {
class ServerApp {
 public:
  enum class State { NONE, INITIALIZING, INITIALIZED, TERMINATED };

 public:
  struct Config {
    std::string appName = "Server";

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
  ServerApp();

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
  FW::Net::TcpListener_ptr tcpListenerApp_;
};
}  // namespace Server
#endif  // #ifndef FW_SERVER_APP_H
