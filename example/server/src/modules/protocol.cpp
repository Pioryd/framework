#include <cmsext/log.h>
#include <cmsext/module.h>
#include <cmsext/net.h>
#include <cmsext/thread.h>

using namespace cmsext::net;
using namespace cmsext::log;
using namespace cmsext::thread;

CMS_BEGIN(protocol_mod, "")

enum ProtocolCodes {
  SERVER_PING = 0x05,
  SERVER_PING_BACK = 0x06,
  CLIENT_PING = 0x07,
  CLIENT_PING_BACK = 0x08,
  SERVER_TEXT_MESSAGE = 0x09,
  CLIENT_TEXT_MESSAGE = 0x0A
};

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
  cms::immutable_dict<int, cms::string> ints = {
      {1, "one"}, {2, "two"}, {3, "three"}};
  {
    cms::immutable_dict<int, cms::string> ints_2;
    ints_2 = ints;
    ints_2[1] = "heelo";
    ints_2.update({{4, "gogo"}});
  }
  cms::immutable_dict<int, cms::string> sec = {{2, "test"}};
  ints.update(sec);
  int32_t i = 0;
  while (i < cms::len(ints)) { 
    info("key: " + cms::str(cms::dict_key(ints, i)));
    info("value: " + cms::dict_value(ints, i));
    i += 1;
  }
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
