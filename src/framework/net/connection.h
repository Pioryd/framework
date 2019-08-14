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
    uint32_t read_timeout = 30;
    uint32_t write_timeout = 30;
    bool debug_enabled = true;
  };

 public:
  Connection(boost::asio::io_service& io_service,
             std::function<void(Connection_ptr&)> on_close,
             std::function<void(Connection_ptr&)> on_connected,
             const PacketParseCallbacks_ptr& packet_parse_callbacks);
  virtual ~Connection();

  void init();

  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;

  void connect(boost::asio::ip::basic_resolver<boost::asio::ip::tcp>::iterator
                   endpoint_iterator);
  void close();
  void send();

  unsigned long get_host();
  std::string get_host_as_string();
  unsigned short get_port();
  std::string get_port_as_string();
  std::string get_info();
  int32_t get_id();
  State get_state();

 protected:
  void read_packet();
  void read_packet_size(const boost::system::error_code& error);
  void read_packet_body(const boost::system::error_code& error);

  void write_packet(const OutputMessage_ptr& msg);

  void on_connect(const boost::system::error_code& error);
  void on_accept();
  void on_write_packet(const boost::system::error_code& error);
  void on_timeout(const boost::system::error_code& error);

  void prepare_to_write(const OutputMessage_ptr& msg) const;
  void parse_packet();

  boost::asio::ip::tcp::socket& getSocket();

 public:
  Config config;

  InputMessage in_msg;
  OutputMessage_ptr out_msg;

  static inline Core::AutoId<Connection> auto_id{1000};
 protected:


  std::function<void(Connection_ptr&)> on_close;
  std::function<void(Connection_ptr&)> on_connected;

  int32_t id_;
  boost::asio::ip::tcp::socket socket_;
  boost::asio::steady_timer read_timer_;
  boost::asio::steady_timer write_timer_;

  std::list<OutputMessage_ptr> output_message_queue_;
  State state_;
  std::recursive_mutex mutex_;

  PacketParseCallbacks_ptr packet_parse_callbacks_;

 private:
  friend class TcpListener;
  friend class TcpClient;
};
}  // namespace FW::Net
#endif  // #ifndef FW_NET_CONNECTION_H
