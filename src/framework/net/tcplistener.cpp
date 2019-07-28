#include "tcplistener.h"

namespace FW::Net {
TcpListener::TcpListener(boost::asio::io_service& io_service,
                         std::function<Protocol_ptr(void)> protocolOnCreate,
                         const std::string& host, uint16_t port,
                         Thread::EventManager& eventManager) {}

TcpListener::~TcpListener() {}

void TcpListener::start() {}

void TcpListener::stop() {}

Connection_ptr TcpListener::createConnection() { return nullptr; }

void TcpListener::onConnectionClose(Connection_ptr& connection) {}

void TcpListener::closeConnections() {}

bool TcpListener::isRunning() { return false; }

void TcpListener::asyncRestart() {}

void TcpListener::asyncAccept() {}

void TcpListener::onAccept(Connection_ptr connection,
                           const boost::system::error_code& error) {}

std::string TcpListener::info() { return ""; }
}  // namespace FW::Net
