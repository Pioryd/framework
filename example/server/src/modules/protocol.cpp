#include <cmsext/cmsext.h>

CMS_BEGIN(protocol_mod, "")

int SERVER_PING = 0x05;
int SERVER_PING_BACK = 0x06;
int CLIENT_PING = 0x07;
int CLIENT_PING_BACK = 0x08;
int SERVER_TEXT_MESSAGE = 0x09;
int CLIENT_TEXT_MESSAGE = 0x0A;

void send_server_ping(uint32_t connection_id) {
  paket_add_uint8(connection_id, SERVER_PING);
  paket_send(connection_id);

  static int i = 0;
  info("send SERVER_PING " + std::to_string(++i));
}
void send_client_ping_back(uint32_t connection_id) {
  paket_add_uint8(connection_id, CLIENT_PING_BACK);
  paket_send(connection_id);

  static int i = 0;
  info("send CLIENT_PING_BACK " + std::to_string(++i));
}

void parse_server_ping_back(uint32_t connection_id) {
  add_sync_event(BIND(send_server_ping, connection_id), 500);
}
void parse_client_ping(uint32_t connection_id) {
  add_sync_event(BIND(send_client_ping_back, connection_id), 500);
}

void on_connected(uint32_t connection_id) { send_server_ping(connection_id); }

void net_test() {
  auto listener_id = tcp_listener_create("127.0.0.1", "7312", on_connected);

  tcp_listener_bind_paket_parse(listener_id, SERVER_PING_BACK,
                                parse_server_ping_back);
  tcp_listener_bind_paket_parse(listener_id, CLIENT_PING, parse_client_ping);
  tcp_listener_start(listener_id);
}
// Signals
void on_load() {}
void on_init() { net_test(); }
void on_terminate() {}
void on_reload() {}

CMS_END()
