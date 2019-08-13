#include "net.h"

#include <framework/global.h>
#include <framework/net/connection.h>
#include <framework/net/manager.h>
#include <framework/net/tcpclient.h>
#include <framework/net/tcplistener.h>

#include <framework/pymodule/manager.h>
#include <pybind11/embed.h>

namespace cmsext::net {
int32_t tcp_listener_create(const cms::string& host, const cms::string& port,
                            void (*on_connected)(uint32_t connection_id)) {
  return FW::G::NetManager.create_tcp_listener(
      [on_connected](FW::Net::Connection_ptr& connection) {
        on_connected(connection->get_id());
      },
      host, port);
}

void tcp_listener_bind_paket_parse(int32_t tcp_listener_id, uint8_t code,
                                   void (*callback)(uint32_t connection_id)) {
  auto listener = FW::G::NetManager.get_tcp_listener(tcp_listener_id);
  listener->packetParseCallbacks->add(
      code, [callback](FW::Net::Connection_ptr& connection) {
        callback(connection->get_id());
      });
}

bool tcp_listener_start(int32_t tcp_listener_id) {
  auto listener = FW::G::NetManager.get_tcp_listener(tcp_listener_id);
  if (!listener) return false;
  listener->start();
  return true;
}

void tcp_listener_get_connections(int32_t tcp_listener_id,
                                  cms::immutable_list<int>& ids) {
  auto listener = FW::G::NetManager.get_tcp_listener(tcp_listener_id);
  if (!listener) return;
  for (auto& connection : listener->get_connections()) {
    ids.append(connection->get_id());
  }
}

int32_t tcp_client_create(const cms::string& host, const cms::string& port,
                          void (*on_connected)(uint32_t connection_id)) {
  return FW::G::NetManager.create_tcp_client(
      [on_connected](FW::Net::Connection_ptr& connection) {
        on_connected(connection->get_id());
      },
      host, port);
}

void tcp_client_bind_paket_parse(int32_t tcp_client_id, uint8_t code,
                                 void (*callback)(uint32_t connection_id)) {
  auto client = FW::G::NetManager.get_tcp_client(tcp_client_id);
  client->packetParseCallbacks->add(
      code, [callback](FW::Net::Connection_ptr& connection) {
        callback(connection->get_id());
      });
}

void tcp_client_connect(int32_t tcp_client_id) {
  auto client = FW::G::NetManager.get_tcp_client(tcp_client_id);
  client->connect();
}

uint8_t paket_get_uint8(int32_t connection_id) {
  auto connection = FW::Net::Connection::auto_id.get_object(connection_id);
  if (!connection) return false;
  return connection->inMsg.getUInt8();
}

void paket_add_uint8(int32_t connection_id, uint8_t value) {
  auto connection = FW::Net::Connection::auto_id.get_object(connection_id);
  if (!connection) return;
  connection->outMsg->putUInt8(value);
}

void paket_send(int32_t connection_id) {
  auto connection = FW::Net::Connection::auto_id.get_object(connection_id);
  if (!connection) return;
  connection->send();
}

bool is_client_connected(int32_t connection_id) {
  auto connection = FW::Net::Connection::auto_id.get_object(connection_id);
  return (connection->get_state() == FW::Net::Connection::State::CONNECTED);
}
}  // namespace cmsext::net

PYBIND11_EMBEDDED_MODULE(cmsext_net, m) {
  m.def("tcp_listener_create", cmsext::net::tcp_listener_create),  //
      m.def("tcp_listener_bind_paket_parse",
            cmsext::net::tcp_listener_bind_paket_parse),
      m.def("tcp_listener_start", cmsext::net::tcp_listener_start),
      m.def("tcp_client_create", cmsext::net::tcp_client_create),
      m.def("tcp_client_bind_paket_parse",
            cmsext::net::tcp_client_bind_paket_parse),
      m.def("tcp_client_connect", cmsext::net::tcp_client_connect),
      m.def("paket_get_uint8", cmsext::net::paket_get_uint8),
      m.def("paket_add_uint8", cmsext::net::paket_add_uint8),
      m.def("paket_send", cmsext::net::paket_send);
}