#ifndef FW_NET_TCPLISTENER_H
#define FW_NET_TCPLISTENER_H

#include <boost/asio.hpp>
#include <unordered_set>
#include "../thread/declarations.h"
#include "../time/declarations.h"
#include "connection.h"
#include "declarations.h"
namespace FW::Net {
class TcpListener : public std::enable_shared_from_this<TcpListener> {
 public:
  enum class State { RUNNING, RESTARTING, STOPPED };

 public:
  struct Config {
    bool enable_restart = true;
    FW::Time::ticks_t restart_per_time = 1000;
    uint32_t packets_per_second = 20;
  };

 public:
  TcpListener(boost::asio::io_service& io_service,
              std::function<void(Connection_ptr&)> on_connected,
              const std::string& host, const std::string& port,
              Thread::EventManager& event_manager);
  ~TcpListener();

  TcpListener(const TcpListener&) = delete;
  TcpListener& operator=(const TcpListener&) = delete;

 public:
  void start();
  void stop();
  bool is_running();

  Connection_ptr create_connection();
  void on_connection_close(Connection_ptr& connection);
  void close_connections();

  Connection_ptr getConnection(uint32_t id);
  std::unordered_set<Connection_ptr>& get_connections();

 private:
  void async_restart();
  void async_accept();
  void on_accept(Connection_ptr connection,
                const boost::system::error_code& error);

  std::string info();

 public:
  Config config;
  Connection::PacketParseCallbacks_ptr packet_parse_callbacks;

 private:
  boost::asio::io_service& io_service_;
  std::string host_;
  std::string port_;
  std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
  std::function<void(Connection_ptr&)> on_connected_;
  State state_;

  std::mutex connections_guard_;
  std::unordered_set<Connection_ptr> connections_;

  Thread::EventManager& event_manager_;
};
}  // namespace FW::Net
#endif  // #ifndef FW_NET_TCPLISTENER_H
