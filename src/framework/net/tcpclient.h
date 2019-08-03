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
    uint32_t readTimeout = 30;
    Time::ticks_t reconnectPerTime = 1000;
    uint32_t packetsPerSecond = 20;
  };

 public:
  TcpClient(boost::asio::io_service& io_service,
            const std::function<void(Connection_ptr&)>& on_connected,
            const std::string& host, const std::string& port,
            Thread::EventManager& eventManager);
  ~TcpClient();

  TcpClient(const TcpClient&) = delete;
  TcpClient& operator=(const TcpClient&) = delete;

 public:
  void connect();
  void disconnect();
  bool isConnected();

 private:
  void onResolve(const boost::system::error_code& error,
                 boost::asio::ip::basic_resolver<boost::asio::ip::tcp>::iterator
                     endpointIterator);
  void asyncReconnect();
  void onTimeout(const boost::system::error_code& error);

  void onConnectionClose(Connection_ptr& connection);

  std::string info();

 public:
  Config config;
  Connection::PacketParseCallbacks_ptr packetParseCallbacks;

 protected:
  boost::asio::io_service& io_service_;
  std::unique_ptr<boost::asio::steady_timer> readTimer_;
  std::unique_ptr<boost::asio::steady_timer> writeTimer_;
  std::unique_ptr<boost::asio::ip::tcp::resolver> resolver_;
  std::string host_;
  std::string port_;
  std::function<void(Connection_ptr&)> on_connected_;
  FW::Net::Connection_ptr connection_;
  State state_;

  Thread::EventManager& eventManager_;
};
}  // namespace FW::Net
#endif  // #ifndef FW_NET_TCPCLIENT_H
