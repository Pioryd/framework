#ifndef FW_CMSEXT_NET_H
#define FW_CMSEXT_NET_H

#include <cmsext/declarations.h>

namespace cmsext::net {
int32_t tcp_listener_create(const cms::string& host, const cms::string& port,
                            void (*on_connected)(uint32_t connection_id));
void tcp_listener_bind_paket_parse(int32_t tcp_listener_id, uint8_t code,
                                      void (*callback)(uint32_t connection_id));
void tcp_listener_start(int32_t tcp_listener_id);

int32_t tcp_client_create(const cms::string& host, const cms::string& port,
                          void (*on_connected)(uint32_t connection_id));
void tcp_client_bind_paket_parse(int32_t tcp_client_id, uint8_t code,
                                    void (*callback)(uint32_t connection_id));
void tcp_client_connect(int32_t tcp_client_id);

uint8_t paket_get_uint8(int32_t connection_id);

void paket_add_uint8(int32_t connection_id, uint8_t value);
void paket_send(int32_t connection_id);
}  // namespace cmsext::net
#endif  // #ifndef FW_CMSEXT_NET_H
