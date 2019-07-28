#ifndef CLIENT_PROTOCOLGAME_H
#define CLIENT_PROTOCOLGAME_H

#include <framework/net/declarations.h>

namespace Client {
namespace ProtocolApp {
FW::Net::Protocol_ptr create();

void sendClientPing(FW::Net::Protocol_ptr protocol);
void sendServerPingBack(FW::Net::Protocol_ptr protocol);

void parseServerTextMessage(FW::Net::Protocol_ptr protocol,
                            FW::Net::InputMessage& msg);
};  // namespace ProtocolApp
}  // namespace Client
#endif  // CLIENT_PROTOCOLGAME_H
