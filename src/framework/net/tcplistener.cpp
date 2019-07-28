#include "tcplistener.h"

#include "../core/logger.h"
#include "../global.h"
#include "../thread/eventmanager.h"
#include "connection.h"
#include "protocol.h"

namespace FW::Net {
TcpListener::TcpListener(boost::asio::io_service& io_service,
                         std::function<Protocol_ptr(void)> protocolOnCreate,
                         const std::string& host, uint16_t port,
                         Thread::EventManager& eventManager)
    : io_service_{io_service},
      protocolOnCreate_{protocolOnCreate},
      host_{host},
      port_{port},
      state_{State::STOPPED},
      eventManager_{eventManager} {
  if (port_ == 0)
    throw std::runtime_error(
        ("Port number 0 is reserved. Select another one. Host: " + host_ + ".")
            .c_str());
}

TcpListener::~TcpListener() { stop(); }

void TcpListener::start() {
  if (state_ != State::STOPPED && state_ != State::RESTARTING) return;

  try {
    boost::asio::ip::address_v4 ipv4_address =
        (host_.empty()) ? boost::asio::ip::address_v4(INADDR_ANY)
                        : boost::asio::ip::address_v4::from_string(host_);

    acceptor_.reset(new boost::asio::ip::tcp::acceptor(
        io_service_, boost::asio::ip::tcp::endpoint(
                         boost::asio::ip::address(ipv4_address), port_)));

    acceptor_->set_option(boost::asio::ip::tcp::no_delay(true));

    asyncAccept();
    state_ = State::RUNNING;
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to start. " + info() + " Error: " + e.what() + ".");
    asyncRestart();
  }
}

void TcpListener::stop() {
  if (acceptor_ && acceptor_->is_open()) {
    boost::system::error_code error;
    acceptor_->close(error);
    if (error)
      G::Logger.error("Unable to close. " + info() +
                      ". Error: " + error.message() + ".");
  }

  state_ = State::STOPPED;
}

Connection_ptr TcpListener::createConnection() {
  G::Logger.info("TcpListener::createConnection()");
  std::lock_guard<std::mutex> lockClass(connectionsGuard_);
  auto protocol = protocolOnCreate_();
  Connection_ptr connection;
  try {
    connection = std::make_shared<Connection>(
        io_service_,
        std::bind(&TcpListener::onConnectionClose, shared_from_this(),
                  std::placeholders::_1),
        protocol, config.packetsPerSecond);
  } catch (const std::exception& e) {
    G::Logger.error("Unable to create connection. Exception: " +
                    std::string(e.what()));
    return nullptr;
  }
  protocol->setConnection(connection);
  connections_.insert(connection);
  return connection;
}

void TcpListener::onConnectionClose(Connection_ptr& connection) {
  G::Logger.info("TcpListener::onConnectionClose()");
  std::lock_guard<std::mutex> lockClass(connectionsGuard_);

  connections_.erase(connection);
}

void TcpListener::closeConnections() {
  std::lock_guard<std::mutex> lockClass(connectionsGuard_);

  for (const auto& connection : connections_) {
    try {
      boost::system::error_code error;
      connection->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                                   error);
      connection->socket_.close(error);
    } catch (boost::system::system_error& e) {
      G::Logger.error("Unable to close connection socket. " + info() +
                      " Error: " + e.what());
    }
  }
  connections_.clear();
}

bool TcpListener::isRunning() { return (state_ == State::RUNNING); }

void TcpListener::asyncRestart() {
  if (!config.enableRestart || state_ == State::RESTARTING) return;

  G::Logger.info("Restarting... " + info());

  stop();
  state_ = State::RESTARTING;
  eventManager_.addAsyncEvent(
      std::bind(&TcpListener::start, shared_from_this()),
      config.restartPerTime);
}

void TcpListener::asyncAccept() {
  G::Logger.info("TcpListener::asyncAccept()");
  if (!acceptor_) return;

  auto connection = createConnection();
  acceptor_->async_accept(connection->getSocket(),
                          std::bind(&TcpListener::onAccept, shared_from_this(),
                                    connection, std::placeholders::_1));
}

void TcpListener::onAccept(Connection_ptr connection,
                           const boost::system::error_code& error) {
  G::Logger.info("TcpListener::onAccept()");
  if (!error) {
    connection->onAccept();
    asyncAccept();
  } else {
    G::Logger.error("Unable to accept connection. " + info() +
                    " Error: " + error.message() + ".");

    if (error != boost::asio::error::operation_aborted) asyncRestart();
  }
}

std::string TcpListener::info() {
  return "Host: " + host_ + ". Port: " + std::to_string(port_) + ".";
}
}  // namespace FW::Net
