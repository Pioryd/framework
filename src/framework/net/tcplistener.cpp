#include "tcplistener.h"

#include "../core/logger.h"
#include "../global.h"
#include "../thread/eventmanager.h"
#include "connection.h"

namespace FW::Net {
TcpListener::TcpListener(boost::asio::io_service& io_service,
                         std::function<void(Connection_ptr&)> on_connected,
                         const std::string& host, const std::string& port,
                         Thread::EventManager& event_manager)
    : io_service_{io_service},
      on_connected_{on_connected},
      host_{host},
      port_{port},
      state_{State::STOPPED},
      event_manager_{event_manager},
      packet_parse_callbacks{
          std::make_shared<Connection::PacketParseCallbacks>()} {}

TcpListener::~TcpListener() { stop(); }

void TcpListener::start() {
  if (state_ != State::STOPPED && state_ != State::RESTARTING) return;

  try {
    boost::asio::ip::address_v4 ipv4_address =
        (host_.empty()) ? boost::asio::ip::address_v4(INADDR_ANY)
                        : boost::asio::ip::address_v4::from_string(host_);

    auto port = atoi(port_.c_str());
    if (port == 0)
      throw std::runtime_error(
          ("Port number 0 is reserved. Select another one. Host: " + host_ +
           ".")
              .c_str());

    acceptor_.reset(new boost::asio::ip::tcp::acceptor(
        io_service_, boost::asio::ip::tcp::endpoint(
                         boost::asio::ip::address(ipv4_address), port)));

    acceptor_->set_option(boost::asio::ip::tcp::no_delay(true));

    async_accept();
    state_ = State::RUNNING;
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to start. " + info() + " Error: " + e.what() + ".");
    async_restart();
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

Connection_ptr TcpListener::create_connection() {
  G::Logger.info("TcpListener::create_connection()");
  std::lock_guard<std::mutex> lockClass(connections_guard_);
  Connection_ptr connection;
  try {
    connection = std::make_shared<Connection>(
        io_service_,
        std::bind(&TcpListener::on_connection_close, shared_from_this(),
                  std::placeholders::_1),
        on_connected_, packet_parse_callbacks);
    connection->init();
  } catch (const std::exception& e) {
    G::Logger.error("Unable to create connection. Exception: " +
                    std::string(e.what()));
    return nullptr;
  }
  connections_.insert(connection);
  return connection;
}

void TcpListener::on_connection_close(Connection_ptr& connection) {
  G::Logger.info("TcpListener::on_connection_close()");
  std::lock_guard<std::mutex> lockClass(connections_guard_);

  connections_.erase(connection);
}

void TcpListener::close_connections() {
  std::lock_guard<std::mutex> lockClass(connections_guard_);

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

Connection_ptr TcpListener::getConnection(uint32_t id) {
  for (auto& connection : connections_)
    if (connection->get_id() == id) return connection;
  return nullptr;
}

std::unordered_set<Connection_ptr>& TcpListener::get_connections() {
  return connections_;
}

bool TcpListener::is_running() { return (state_ == State::RUNNING); }

void TcpListener::async_restart() {
  if (!config.enable_restart || state_ == State::RESTARTING) return;

  G::Logger.info("Restarting... " + info());

  stop();
  state_ = State::RESTARTING;
  event_manager_.add_async_event(
      std::bind(&TcpListener::start, shared_from_this()),
      config.restart_per_time);
}

void TcpListener::async_accept() {
  G::Logger.info("TcpListener::async_accept()");
  if (!acceptor_) return;

  auto connection = create_connection();
  acceptor_->async_accept(connection->getSocket(),
                          std::bind(&TcpListener::on_accept, shared_from_this(),
                                    connection, std::placeholders::_1));
}

void TcpListener::on_accept(Connection_ptr connection,
                           const boost::system::error_code& error) {
  G::Logger.info("TcpListener::on_accept()");
  if (!error) {
    connection->on_accept();
    async_accept();
  } else {
    G::Logger.error("Unable to accept connection. " + info() +
                    " Error: " + error.message() + ".");

    if (error != boost::asio::error::operation_aborted) async_restart();
  }
}

std::string TcpListener::info() {
  return "Host: " + host_ + ". Port: " + port_ + ".";
}
}  // namespace FW::Net
