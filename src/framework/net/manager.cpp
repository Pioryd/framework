#include "manager.h"

#include "../core/application.h"
#include "../global.h"
#include "../net/tcpclient.h"
#include "../net/tcplistener.h"
#include "../thread/eventmanager.h"

namespace FW::Net {
int32_t Manager::create_tcp_listener(
    std::function<void(Connection_ptr&)> on_connected, const std::string& host,
    const std::string& port) {
  tcp_listeners_.push_back(std::make_shared<FW::Net::TcpListener>(
      FW::G::Application.force()->get_io_service(), on_connected, host, port,
      FW::G::EventManager));
  return tcp_listeners_.size() - 1;
}

int32_t Manager::create_tcp_client(
    std::function<void(Connection_ptr&)> on_connected, const std::string& host,
    const std::string& port) {
  tcp_clients_.push_back(std::make_shared<FW::Net::TcpClient>(
      FW::G::Application.force()->get_io_service(), on_connected, host, port,
      FW::G::EventManager));
  return tcp_clients_.size() - 1;
}

TcpListener_ptr Manager::get_tcp_listener(int32_t id) {
  if (id >= tcp_listeners_.size()) return nullptr;
  return tcp_listeners_[id];
}

TcpClient_ptr Manager::get_tcp_client(int32_t id) {
  if (id >= tcp_clients_.size()) return nullptr;
  return tcp_clients_[id];
}
}  // namespace FW::Net
