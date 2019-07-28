#ifndef FW_NET_CONNECTION_H
#define FW_NET_CONNECTION_H

#include "../thread/declarations.h"
#include "declarations.h"

#include "inputmessage.h"
#include "outputmessage.h"

#include <boost/asio.hpp>

namespace FW::Net {
class Connection : public std::enable_shared_from_this<Connection> {
 public:
  enum class State { OPEN, CLOSED, CONNECTING, CONNECTED };

 public:
  struct Config {
    uint32_t readTimeout = 30;
    uint32_t writeTimeout = 30;
    bool debug_enabled = true;
  };

 public:
  Connection(boost::asio::io_service& io_service,
             std::function<void(Connection_ptr&)> onClose,
             const Protocol_ptr& protocol, uint32_t packetsPerSecond);
  virtual ~Connection();

  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;

  void connect(boost::asio::ip::basic_resolver<boost::asio::ip::tcp>::iterator
                   endpointIterator);
  void close();
  void send(const OutputMessage_ptr& msg);

  unsigned long getHost();
  std::string getHostAsString();
  unsigned short getPort();
  std::string getPortAsString();
  std::string getInfo();

 protected:
  void readPacket();
  void readPacketSize(const boost::system::error_code& error);
  void readPacketBody(const boost::system::error_code& error);

  void writePacket(const OutputMessage_ptr& msg);

  void onConnect(const boost::system::error_code& error);
  void onAccept();
  void onWritePacket(const boost::system::error_code& error);
  void onTimeout(const boost::system::error_code& error);

  boost::asio::ip::tcp::socket& getSocket();

 public:
  Config config;
  std::function<void(Connection_ptr&)> onClose;

 protected:
  boost::asio::ip::tcp::socket socket_;
  boost::asio::steady_timer readTimer_;
  boost::asio::steady_timer writeTimer_;
  OutputMessage outMsg_;
  InputMessage inMsg_;
  std::list<OutputMessage_ptr> outputMessageQueue_;
  Protocol_ptr protocol_;
  State state_;
  std::recursive_mutex mutex_;

 private:
  friend class TcpListener;
  friend class TcpClient;
};
}  // namespace FW::Net
#endif  // #ifndef FW_NET_CONNECTION_H
