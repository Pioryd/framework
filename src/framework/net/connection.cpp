#include "connection.h"

#include "../core/logger.h"
#include "../global.h"

#include "protocol.h"
#include "tcplistener.h"

namespace FW::Net {
Connection::Connection(boost::asio::io_service& io_service,
                       std::function<void(Connection_ptr&)> onClose,
                       const Protocol_ptr& protocol,
                       uint32_t packetsPerSecond) {}

Connection::~Connection() {}

void Connection::connect(
    boost::asio::ip::basic_resolver<boost::asio::ip::tcp>::iterator
        endpointIterator) {}

void Connection::close() {}

void Connection::send(const OutputMessage_ptr& msg) {}

unsigned long Connection::getHost() { return 0; }

std::string Connection::getHostAsString() { return ""; }

unsigned short Connection::getPort() { return 0; }

std::string Connection::getPortAsString() { return ""; }

std::string Connection::getInfo() { return ""; }

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
