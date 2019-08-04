#ifndef FW_NET_CONNECTION_H
#define FW_NET_CONNECTION_H

#include "../core/autoid.h"
#include "../thread/declarations.h"
#include "declarations.h"

#include "inputmessage.h"
#include "outputmessage.h"

#include <boost/asio.hpp>

namespace FW::Net {
class Connection : public std::enable_shared_from_this<Connection> {
 public:
  struct PacketParseCallbacks {
    void add(uint8_t code, std::function<void(Connection_ptr&)> callback) {
      std::lock_guard<std::recursive_mutex> lock(mutex_);
      if (callbacks_.count(code))
        throw std::runtime_error("Packet parse callback already binded: " +
                                 std::to_string(code));
      callbacks_[code] = callback;
    }

    void call(uint8_t code, Connection_ptr& connection) {
      std::lock_guard<std::recursive_mutex> lock(mutex_);
      if (callbacks_.count(code)) callbacks_[code](connection);
    }

   protected:
    std::recursive_mutex mutex_;
    std::map<uint8_t, std::function<void(Connection_ptr&)>> callbacks_;
  };
  typedef std::shared_ptr<PacketParseCallbacks> PacketParseCallbacks_ptr;

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
             std::function<void(Connection_ptr&)> on_connected,
             const PacketParseCallbacks_ptr& packetParseCallbacks);
  virtual ~Connection();

  void init();

  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;

  void connect(boost::asio::ip::basic_resolver<boost::asio::ip::tcp>::iterator
                   endpointIterator);
  void close();
  void send();

  unsigned long getHost();
  std::string getHostAsString();
  unsigned short getPort();
  std::string getPortAsString();
  std::string getInfo();
  int32_t get_id();
  State get_state();

 protected:
  void readPacket();
  void readPacketSize(const boost::system::error_code& error);
  void readPacketBody(const boost::system::error_code& error);

  void writePacket(const OutputMessage_ptr& msg);

  void onConnect(const boost::system::error_code& error);
  void onAccept();
  void onWritePacket(const boost::system::error_code& error);
  void onTimeout(const boost::system::error_code& error);

  void prepareToWrite(const OutputMessage_ptr& msg) const;
  void parsePacket();

  boost::asio::ip::tcp::socket& getSocket();

 public:
  Config config;

  InputMessage inMsg;
  OutputMessage_ptr outMsg;

  static inline Core::AutoId<Connection> auto_id{1000};
 protected:


  std::function<void(Connection_ptr&)> onClose;
  std::function<void(Connection_ptr&)> on_connected;

  int32_t id_;
  boost::asio::ip::tcp::socket socket_;
  boost::asio::steady_timer readTimer_;
  boost::asio::steady_timer writeTimer_;

  std::list<OutputMessage_ptr> outputMessageQueue_;
  State state_;
  std::recursive_mutex mutex_;

  PacketParseCallbacks_ptr packetParseCallbacks_;

 private:
  friend class TcpListener;
  friend class TcpClient;
};
}  // namespace FW::Net
#endif  // #ifndef FW_NET_CONNECTION_H
