#include <cmsext/cmsext.h>

CMS_BEGIN(protocol, "")

int SERVER_PING = 0x05;
int SERVER_PING_BACK = 0x06;
int CLIENT_PING = 0x07;
int CLIENT_PING_BACK = 0x08;
int SERVER_TEXT_MESSAGE = 0x09;
int CLIENT_TEXT_MESSAGE = 0x0A;

void send_client_ping(uint32_t connection_id) {
  paket_add_uint8(connection_id, CLIENT_PING);
  paket_send(connection_id);

  static int i = 0;
  info("send CLIENT_PING " + std::to_string(++i));
}
void send_serer_ping_back(uint32_t connection_id) {
  paket_add_uint8(connection_id, SERVER_PING_BACK);
  paket_send(connection_id);

  static int i = 0;
  info("send SERVER_PING_BACK " + std::to_string(++i));
}

void parse_client_ping_back(uint32_t connection_id) {
  add_sync_event(BIND(send_client_ping, connection_id), 500);
}
void parse_server_ping(uint32_t connection_id) {
  add_sync_event(BIND(send_serer_ping_back, connection_id), 500);
}

void on_connected(uint32_t connection_id) { send_client_ping(connection_id); }

void net_test() {
  auto client_id = tcp_client_create("127.0.0.1", "7312", on_connected);

  tcp_client_bind_paket_parse(client_id, CLIENT_PING_BACK,
                              parse_client_ping_back);
  tcp_client_bind_paket_parse(client_id, SERVER_PING, parse_server_ping);
  tcp_client_connect(client_id);
}
// Signals
void on_load() {}
void on_init() { net_test(); }
void on_terminate() {}
void on_reload() {}

CMS_END()
