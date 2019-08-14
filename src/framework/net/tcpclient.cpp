#include "tcpclient.h"

#include "../core/logger.h"
#include "../global.h"
#include "../thread/eventmanager.h"
#include "connection.h"

namespace FW::Net {
TcpClient::TcpClient(boost::asio::io_service& io_service,
                     const std::function<void(Connection_ptr&)>& on_connected,
                     const std::string& host, const std::string& port,
                     Thread::EventManager& event_manager)
    : io_service_{io_service},
      on_connected_{on_connected},
      host_{host},
      port_{port},
      state_{State::DISCONNECTED},
      event_manager_{event_manager},
      packet_parse_callbacks{
          std::make_shared<Connection::PacketParseCallbacks>()} {}

TcpClient::~TcpClient() { disconnect(); }

void TcpClient::connect() {
  if (state_ != State::DISCONNECTED && state_ != State::RECONNECTING) return;

  state_ = State::CONNECTING;

  try {
    read_timer_ = std::make_unique<boost::asio::steady_timer>(io_service_);
    write_timer_ = std::make_unique<boost::asio::steady_timer>(io_service_);

    connection_ = std::make_shared<Connection>(
        io_service_,
        std::bind(&TcpClient::on_connection_close, shared_from_this(),
                  std::placeholders::_1),
        on_connected_, packet_parse_callbacks);
    connection_->init();

    resolver_ = std::make_unique<boost::asio::ip::tcp::resolver>(io_service_);

    if (port_ == "0")
      throw std::runtime_error(
          ("Port number 0 is reserved. Select another one. Host: " + host_ +
           ".")
              .c_str());

    resolver_->async_resolve(
        boost::asio::ip::tcp::resolver::query(host_, port_),
        std::bind(&TcpClient::on_resolve, shared_from_this(),
                  std::placeholders::_1, std::placeholders::_2));

    read_timer_->cancel();
    read_timer_->expires_from_now(std::chrono::seconds(config.read_timeout));
    read_timer_->async_wait(std::bind(&TcpClient::on_timeout, shared_from_this(),
                                     std::placeholders::_1));
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to connect. " + info() + " Error: " + e.what() +
                    ".");
    async_reconnect();
  }
}

void TcpClient::disconnect() {
  try {
    if (read_timer_) read_timer_->cancel();
    if (write_timer_) write_timer_->cancel();

    if (connection_) connection_->close();
    if (resolver_) resolver_->cancel();

    read_timer_ = nullptr;
    write_timer_ = nullptr;

    connection_ = nullptr;
    resolver_ = nullptr;

    state_ = State::DISCONNECTED;
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to disconnect. " + info() + " Error: " + e.what() +
                    ".");
  }
}

bool TcpClient::is_connected() { return (state_ == State::CONNECTED); }

void TcpClient::on_resolve(
    const boost::system::error_code& error,
    boost::asio::ip::basic_resolver<boost::asio::ip::tcp>::iterator
        endpoint_iterator) {
  read_timer_->cancel();

  if (error) {
    G::Logger.error("Unable to accept connection. " + info() +
                    " Error: " + error.message() + ".");

    if (error != boost::asio::error::operation_aborted) async_reconnect();
    return;
  }

  try {
    connection_->connect(endpoint_iterator);
    state_ = State::CONNECTED;
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to socket connect. " + info() +
                    " Error: " + e.what() + ".");
    async_reconnect();
  }
}

void TcpClient::async_reconnect() {
  if (state_ == State::RECONNECTING) return;

  G::Logger.info("Reconnecting...");

  try {
    disconnect();

    state_ = State::RECONNECTING;
    event_manager_.add_async_event(
        std::bind(&TcpClient::connect, shared_from_this()),
        config.reconnect_per_time);
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to reconnect. " + info() + " Error: " + e.what() +
                    ".");

    std::this_thread::sleep_for(
        std::chrono::milliseconds(config.reconnect_per_time));
    async_reconnect();
  }
}

void TcpClient::on_timeout(const boost::system::error_code& error) {
  G::Logger.error("Timeout: [" + host_ + ":" + port_ +
                  "]. Error: " + error.message() + ".");
  if (error != boost::asio::error::operation_aborted) async_reconnect();
}

void TcpClient::on_connection_close(Connection_ptr& connection) {
  connection_ = nullptr;
}

std::string TcpClient::info() {
  return "Host: " + host_ + ". Port: " + port_ + ".";
}
}  // namespace FW::Net
