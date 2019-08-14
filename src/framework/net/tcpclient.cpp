#include "tcpclient.h"

#include "../core/logger.h"
#include "../global.h"
#include "../thread/eventmanager.h"
#include "connection.h"

namespace FW::Net {
TcpClient::TcpClient(boost::asio::io_service& io_service,
                     const std::function<void(Connection_ptr&)>& on_connected,
                     const std::string& host, const std::string& port,
                     Thread::EventManager& eventManager)
    : io_service_{io_service},
      on_connected_{on_connected},
      host_{host},
      port_{port},
      state_{State::DISCONNECTED},
      eventManager_{eventManager},
      packetParseCallbacks{
          std::make_shared<Connection::PacketParseCallbacks>()} {}

TcpClient::~TcpClient() { disconnect(); }

void TcpClient::connect() {
  if (state_ != State::DISCONNECTED && state_ != State::RECONNECTING) return;

  state_ = State::CONNECTING;

  try {
    readTimer_ = std::make_unique<boost::asio::steady_timer>(io_service_);
    writeTimer_ = std::make_unique<boost::asio::steady_timer>(io_service_);

    connection_ = std::make_shared<Connection>(
        io_service_,
        std::bind(&TcpClient::onConnectionClose, shared_from_this(),
                  std::placeholders::_1),
        on_connected_, packetParseCallbacks);
    connection_->init();

    resolver_ = std::make_unique<boost::asio::ip::tcp::resolver>(io_service_);

    if (port_ == "0")
      throw std::runtime_error(
          ("Port number 0 is reserved. Select another one. Host: " + host_ +
           ".")
              .c_str());

    resolver_->async_resolve(
        boost::asio::ip::tcp::resolver::query(host_, port_),
        std::bind(&TcpClient::onResolve, shared_from_this(),
                  std::placeholders::_1, std::placeholders::_2));

    readTimer_->cancel();
    readTimer_->expires_from_now(std::chrono::seconds(config.readTimeout));
    readTimer_->async_wait(std::bind(&TcpClient::onTimeout, shared_from_this(),
                                     std::placeholders::_1));
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to connect. " + info() + " Error: " + e.what() +
                    ".");
    asyncReconnect();
  }
}

void TcpClient::disconnect() {
  try {
    if (readTimer_) readTimer_->cancel();
    if (writeTimer_) writeTimer_->cancel();

    if (connection_) connection_->close();
    if (resolver_) resolver_->cancel();

    readTimer_ = nullptr;
    writeTimer_ = nullptr;

    connection_ = nullptr;
    resolver_ = nullptr;

    state_ = State::DISCONNECTED;
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to disconnect. " + info() + " Error: " + e.what() +
                    ".");
  }
}

bool TcpClient::isConnected() { return (state_ == State::CONNECTED); }

void TcpClient::onResolve(
    const boost::system::error_code& error,
    boost::asio::ip::basic_resolver<boost::asio::ip::tcp>::iterator
        endpointIterator) {
  readTimer_->cancel();

  if (error) {
    G::Logger.error("Unable to accept connection. " + info() +
                    " Error: " + error.message() + ".");

    if (error != boost::asio::error::operation_aborted) asyncReconnect();
    return;
  }

  try {
    connection_->connect(endpointIterator);
    state_ = State::CONNECTED;
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to socket connect. " + info() +
                    " Error: " + e.what() + ".");
    asyncReconnect();
  }
}

void TcpClient::asyncReconnect() {
  if (state_ == State::RECONNECTING) return;

  G::Logger.info("Reconnecting...");

  try {
    disconnect();

    state_ = State::RECONNECTING;
    eventManager_.add_async_event(
        std::bind(&TcpClient::connect, shared_from_this()),
        config.reconnectPerTime);
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to reconnect. " + info() + " Error: " + e.what() +
                    ".");

    std::this_thread::sleep_for(
        std::chrono::milliseconds(config.reconnectPerTime));
    asyncReconnect();
  }
}

void TcpClient::onTimeout(const boost::system::error_code& error) {
  G::Logger.error("Timeout: [" + host_ + ":" + port_ +
                  "]. Error: " + error.message() + ".");
  if (error != boost::asio::error::operation_aborted) asyncReconnect();
}

void TcpClient::onConnectionClose(Connection_ptr& connection) {
  connection_ = nullptr;
}

std::string TcpClient::info() {
  return "Host: " + host_ + ". Port: " + port_ + ".";
}
}  // namespace FW::Net
