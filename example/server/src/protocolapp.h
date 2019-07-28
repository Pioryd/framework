#ifndef SERVER_PROTOCOL_APP_H
#define SERVER_PROTOCOL_APP_H

#include <framework/net/declarations.h>

namespace Server {
namespace ProtocolApp {
FW::Net::Protocol_ptr create();

void sendServerPing(FW::Net::Protocol_ptr protocol);
void sendClientPingBack(FW::Net::Protocol_ptr protocol);
void sendTextMessage(FW::Net::Protocol_ptr protocol,
                     const std::string& message);

void parseClientTextMessage(FW::Net::Protocol_ptr protocol,
                            FW::Net::InputMessage& msg);
};  // namespace ProtocolApp
}  // namespace Server
#endif  // SERVER_PROTOCOL_APP_H
