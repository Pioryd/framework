#ifndef FW_NET_TCPCLIENT_H
#define FW_NET_TCPCLIENT_H

#include <boost/asio.hpp>
#include <unordered_set>
#include "../thread/declarations.h"
#include "../time/declarations.h"
#include "connection.h"
#include "declarations.h"

namespace FW::Net {
class TcpClient : public std::enable_shared_from_this<TcpClient> {
 public:
  enum class State { DISCONNECTED, CONNECTING, CONNECTED, RECONNECTING };

 public:
  struct Config {
    uint32_t read_timeout = 30;
    Time::ticks_t reconnect_per_time = 1000;
    uint32_t packets_per_second = 20;
  };

 public:
  TcpClient(boost::asio::io_service& io_service,
            const std::function<void(Connection_ptr&)>& on_connected,
            const std::string& host, const std::string& port,
            Thread::EventManager& event_manager);
  ~TcpClient();

  TcpClient(const TcpClient&) = delete;
  TcpClient& operator=(const TcpClient&) = delete;

 public:
  void connect();
  void disconnect();
  bool is_connected();

 private:
  void on_resolve(const boost::system::error_code& error,
                 boost::asio::ip::basic_resolver<boost::asio::ip::tcp>::iterator
                     endpoint_iterator);
  void async_reconnect();
  void on_timeout(const boost::system::error_code& error);

  void on_connection_close(Connection_ptr& connection);

  std::string info();

 public:
  Config config;
  Connection::PacketParseCallbacks_ptr packet_parse_callbacks;

 protected:
  boost::asio::io_service& io_service_;
  std::unique_ptr<boost::asio::steady_timer> read_timer_;
  std::unique_ptr<boost::asio::steady_timer> write_timer_;
  std::unique_ptr<boost::asio::ip::tcp::resolver> resolver_;
  std::string host_;
  std::string port_;
  std::function<void(Connection_ptr&)> on_connected_;
  FW::Net::Connection_ptr connection_;
  State state_;

  Thread::EventManager& event_manager_;
};
}  // namespace FW::Net
#endif  // #ifndef FW_NET_TCPCLIENT_H
