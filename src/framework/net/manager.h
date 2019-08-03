#ifndef FW_NET_MANAGER_H
#define FW_NET_MANAGER_H

#include "declarations.h"

namespace FW::Net {
class Manager {
 public:
  int32_t create_tcp_listener(std::function<void(Connection_ptr&)> on_connected,
                              const std::string& host, const std::string& port);

  int32_t create_tcp_client(std::function<void(Connection_ptr&)> on_connected,
                            const std::string& host, const std::string& port);

  TcpListener_ptr get_tcp_listener(int32_t id);
  TcpClient_ptr get_tcp_client(int32_t id);

 protected:
  std::vector<TcpListener_ptr> tcp_listeners_;
  std::vector<TcpClient_ptr> tcp_clients_;
};
}  // namespace FW::Net
#endif  // #ifndef FW_NET_MANAGER_H
