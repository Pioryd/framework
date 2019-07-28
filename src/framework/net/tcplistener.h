#ifndef FW_NET_TCPLISTENER_H
#define FW_NET_TCPLISTENER_H

#include "declarations.h"

#include <boost/asio.hpp>
#include <unordered_set>
#include "../thread/declarations.h"
#include "../time/declarations.h"

namespace FW::Net {
class TcpListener : public std::enable_shared_from_this<TcpListener> {
 public:
  enum class State { RUNNING, RESTARTING, STOPPED };

 public:
  struct Config {
    bool enableRestart = true;
    FW::Time::ticks_t restartPerTime = 1000;
    uint32_t packetsPerSecond = 20;
  };

 public:
  TcpListener(boost::asio::io_service& io_service,
              std::function<Protocol_ptr(void)> protocolOnCreate,
              const std::string& host, uint16_t port,
              Thread::EventManager& eventManager);
  ~TcpListener();

  TcpListener(const TcpListener&) = delete;
  TcpListener& operator=(const TcpListener&) = delete;

 public:
  void start();
  void stop();
  bool isRunning();

  Connection_ptr createConnection();
  void onConnectionClose(Connection_ptr& connection);
  void closeConnections();

 private:
  void asyncRestart();
  void asyncAccept();
  void onAccept(Connection_ptr connection,
                const boost::system::error_code& error);

  std::string info();

 public:
  Config config;

 private:
  boost::asio::io_service& io_service_;
  std::string host_;
  uint16_t port_;
  std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
  std::function<Protocol_ptr(void)> protocolOnCreate_;
  State state_;

  std::mutex connectionsGuard_;
  std::unordered_set<Connection_ptr> connections_;

  Thread::EventManager& eventManager_;
};
}  // namespace FW::Net
#endif  // #ifndef FW_NET_TCPLISTENER_H
