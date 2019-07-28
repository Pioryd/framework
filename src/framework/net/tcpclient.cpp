#include "tcpclient.h"

namespace FW::Net {
TcpClient::TcpClient(boost::asio::io_service& io_service,
                     const std::function<Protocol_ptr(void)>& protocolOnCreate,
                     const std::string& host, uint16_t port,
                     Thread::EventManager& eventManager) {}

TcpClient::~TcpClient() {}

void TcpClient::connect() {}

void TcpClient::disconnect() {}

bool TcpClient::isConnected() { return false; }

void TcpClient::onResolve(
    const boost::system::error_code& error,
    boost::asio::ip::basic_resolver<boost::asio::ip::tcp>::iterator
        endpointIterator) {}

void TcpClient::asyncReconnect() {}

void TcpClient::onTimeout(const boost::system::error_code& error) {}

void TcpClient::onConnectionClose(Connection_ptr& connection) {}

std::string TcpClient::info() { return ""; }
}  // namespace FW::Net
