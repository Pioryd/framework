#include "connection.h"

#include "../core/logger.h"
#include "../global.h"

#include "protocol.h"
#include "tcplistener.h"

namespace FW::Net {
Connection::Connection(boost::asio::io_service& io_service,
                       std::function<void(Connection_ptr&)> onClose,
                       const Protocol_ptr& protocol, uint32_t packetsPerSecond)
    : onClose{onClose},
      protocol_{protocol},
      socket_{io_service},
      readTimer_{io_service},
      writeTimer_{io_service},
      state_{State::OPEN} {
  if (!protocol_)
    throw std::runtime_error(
        (getInfo() + " Unable to read packet body. Protocol is NOT set.")
            .c_str());
}

Connection::~Connection() { close(); }

void Connection::connect(
    boost::asio::ip::basic_resolver<boost::asio::ip::tcp>::iterator
        endpointIterator) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);

  if (state_ == State::CLOSED) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() + " Unable to connect. Connection is closed.");)
    return;
  }

  if (state_ == State::CONNECTING || state_ == State::CONNECTED) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() + " Unable to connect. Connection is connecting/connected.");)
    return;
  }

  state_ = State::CONNECTING;

  try {
    readTimer_.cancel();
    readTimer_.expires_from_now(std::chrono::seconds(config.readTimeout));
    readTimer_.async_wait(std::bind(&Connection::onTimeout, shared_from_this(),
                                    std::placeholders::_1));

    socket_.async_connect(*endpointIterator,
                          std::bind(&Connection::onConnect, shared_from_this(),
                                    std::placeholders::_1));
  } catch (boost::system::system_error& e) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() + " Unable to connect. Error: " + std::string(e.what()) +
        ".");)
    close();
  }
}

void Connection::close() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (state_ == State::CLOSED) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() + " Unable to close connection. Connection is closed.");)
    return;
  }

  auto this_ptr = shared_from_this();
  onClose(this_ptr);

  if (socket_.is_open()) {
    try {
      readTimer_.cancel();
      writeTimer_.cancel();
      socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
      socket_.close();
    } catch (boost::system::system_error& e) {
      G::Logger.error(getInfo() +
                      " Unable to correctly close connection. Error: " +
                      std::string(e.what()) + ".");
    }
  }

  protocol_ = nullptr;

  state_ = State::CLOSED;
}

void Connection::send(const OutputMessage_ptr& msg) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (state_ != State::CONNECTED) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() + " Unable to send. Connection is NOT connected.");)
    return;
  }

  bool isSocketWritingPackets = outputMessageQueue_.empty();
  outputMessageQueue_.emplace_back(msg);
  if (isSocketWritingPackets) { writePacket(msg); }
}

unsigned long Connection::getHost() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  try {
    return socket_.remote_endpoint().address().to_v4().to_ulong();
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to get ip. Error: " + std::string(e.what()));
  }
  return 0;
}

std::string Connection::getHostAsString() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  try {
    return socket_.remote_endpoint().address().to_string();
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to get ip. Error: " + std::string(e.what()));
  }
  return "";
}

unsigned short Connection::getPort() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  try {
    return socket_.remote_endpoint().port();
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to get ip. Error: " + std::string(e.what()));
  }
  return 0;
}

std::string Connection::getPortAsString() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  return std::to_string(getPort());
}

std::string Connection::getInfo() {
  // TODO BUG
  return std::string("std::string Connection::getInfo()"
    /*"Connection[Host: " + getHostAsString() + " Port: " +
                     getPortAsString() + 
                     " Protocol: " + protocol_->name + "]"*/);
}

void Connection::readPacket() {}

void Connection::readPacketSize(const boost::system::error_code& error) {}

void Connection::readPacketBody(const boost::system::error_code& error) {}

void Connection::writePacket(const OutputMessage_ptr& msg) {}

void Connection::onConnect(const boost::system::error_code& error) {}

void Connection::onAccept() {}

void Connection::onWritePacket(const boost::system::error_code& error) {}

void Connection::onTimeout(const boost::system::error_code& error) {}

boost::asio::ip::tcp::socket& Connection::getSocket() { return nullptr; }
}  // namespace FW::Net
