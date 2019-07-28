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

void Connection::readPacket() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  try {
    readTimer_.cancel();

    if (state_ != State::CONNECTED) {
      FW_DEBUG_INSTRUCTIONS(G::Logger.error(
          getInfo() + " Unable to read packet. Connection is NOT connected.");)
      return;
    }

    readTimer_.expires_from_now(std::chrono::seconds(config.readTimeout));
    readTimer_.async_wait(std::bind(&Connection::onTimeout, shared_from_this(),
                                    std::placeholders::_1));

    inMsg_.reset();
    inMsg_.setLength(NetworkMessage::PACKET_SIZE_BYTES_LENGTH);

    boost::asio::async_read(
        socket_,
        boost::asio::buffer(inMsg_.getBufferAtCurrentPosition(),
                            NetworkMessage::PACKET_SIZE_BYTES_LENGTH),
        std::bind(&Connection::readPacketSize, shared_from_this(),
                  std::placeholders::_1));
  } catch (boost::system::system_error& e) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() + " Unable to read packet. Error: " + std::string(e.what()) +
        ".");)
    close();
  }
}

void Connection::readPacketSize(const boost::system::error_code& error) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  try {
    readTimer_.cancel();

    if (state_ != State::CONNECTED) {
      FW_DEBUG_INSTRUCTIONS(G::Logger.error(
          getInfo() +
          " Unable to read packet size. Connection is NOT connected.");)
      return;
    }

    if (error) {
      FW_DEBUG_INSTRUCTIONS(G::Logger.error(
          getInfo() + " Unable to read packet size. Error: " + error.message() +
          ".");)
      close();
      return;
    }

    inMsg_.setLength(inMsg_.peekUInt16());
    if (inMsg_.getLength() == 0 ||
        inMsg_.getLength() >= NetworkMessage::MAX_PACKET_BODY_LENGTH) {
      FW_DEBUG_INSTRUCTIONS(G::Logger.error(
          getInfo() +
          " Wrong packet size: " + std::to_string(inMsg_.getLength()) + ".");)
      close();
      return;
    }

    readTimer_.expires_from_now(std::chrono::seconds(config.readTimeout));
    readTimer_.async_wait(std::bind(&Connection::onTimeout, shared_from_this(),
                                    std::placeholders::_1));

    boost::asio::async_read(
        socket_,
        boost::asio::buffer(inMsg_.getBufferAtCurrentPosition(),
                            inMsg_.getLength() - inMsg_.getBufferPosition()),
        std::bind(&Connection::readPacketBody, shared_from_this(),
                  std::placeholders::_1));
  } catch (boost::system::system_error& e) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() +
        " Unable to read packet size. Error: " + std::string(e.what()));)
    close();
  }
}

void Connection::readPacketBody(const boost::system::error_code& error) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  try {
    readTimer_.cancel();
  } catch (boost::system::system_error& e) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() +
        " Unable to read packet body. Error: " + std::string(e.what()));)
    close();
  }

  if (error) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() + " Unable to parse packet. Error: " + error.message() +
        ".");)
    close();
    return;
  }

  if (state_ != State::CONNECTED) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() +
        " Unable to read packet body. Connection is NOT connected.");)
    return;
  }

  protocol_->onRecvMessage(inMsg_);

  readPacket();
}

void Connection::writePacket(const OutputMessage_ptr& msg) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  try {
    writeTimer_.cancel();

    if (state_ != State::CONNECTED) {
      FW_DEBUG_INSTRUCTIONS(G::Logger.error(
          getInfo() + " Unable to write packet. Connection is NOT connected.");)
      return;
    }

    protocol_->prepareToWrite(msg);

    writeTimer_.expires_from_now(std::chrono::seconds(config.writeTimeout));
    writeTimer_.async_wait(std::bind(&Connection::onTimeout, shared_from_this(),
                                     std::placeholders::_1));

    boost::asio::async_write(
        socket_, boost::asio::buffer(msg->getOutputBuffer(), msg->getLength()),
        std::bind(&Connection::onWritePacket, shared_from_this(),
                  std::placeholders::_1));
  } catch (boost::system::system_error& e) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() +
        " Unable to write packet. Error: " + std::string(e.what()));)
    close();
  }
}

void Connection::onConnect(const boost::system::error_code& error) {
  try {
    readTimer_.cancel();
  } catch (boost::system::system_error& e) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() +
        " Unable to handle connect. Error: " + std::string(e.what()));)
    close();
  }

  if (error == boost::asio::error::operation_aborted) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() + " Unable to handle connect. Operation aborted.");)
    return;
  }

  if (error) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() + " Unable to handle connect. Error: " + error.message() +
        ".");)
    close();
    return;
  }

  boost::asio::ip::tcp::no_delay option(true);
  socket_.set_option(option);
  state_ = State::CONNECTED;
  protocol_->onConnected(protocol_);
  readPacket();
}

void Connection::onAccept() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  state_ = State::CONNECTED;
  protocol_->onConnected(protocol_);
  readPacket();
}

void Connection::onWritePacket(const boost::system::error_code& error) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  writeTimer_.cancel();
  outputMessageQueue_.pop_front();

  if (error) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() + " Error while writing packet. Error: " + error.message() +
        ".");)
    outputMessageQueue_.clear();
    close();
    return;
  }

  if (state_ != State::CONNECTED) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        getInfo() +
        " Unable to handle write packet. Connection is NOT connected.");)
    return;
  }

  if (!outputMessageQueue_.empty()) writePacket(outputMessageQueue_.front());
}

void Connection::onTimeout(const boost::system::error_code& error) {
  if (error == boost::asio::error::operation_aborted) {
    // FW_DEBUG_INSTRUCTIONS(
    //    G::Logger.info(getInfo() + " Timeout. Operation aborted.");)
    return;
  }

  FW_DEBUG_INSTRUCTIONS(
      G::Logger.error(getInfo() + " Timeout. Error: " + error.message() + ".");)
  close();
}

boost::asio::ip::tcp::socket& Connection::getSocket() { return socket_; }
}  // namespace FW::Net
