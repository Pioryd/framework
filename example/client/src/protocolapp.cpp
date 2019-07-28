#include "protocolapp.h"

#include <framework/core/logger.h>
#include <framework/global.h>
#include <framework/net/protocol.h>

namespace Client::ProtocolApp::Private {
void onConnected(FW::Net::Protocol_ptr& protocol);
void onPrsePacket(FW::Net::Protocol_ptr& protocol, FW::Net::InputMessage& msg);
}  // namespace Client::ProtocolApp::Private

namespace Client {
enum ProtocolCodes {
  SERVER_PING = 0x05,
  SERVER_PING_BACK = 0x06,
  CLIENT_PING = 0x07,
  CLIENT_PING_BACK = 0x08,
  SERVER_TEXT_MESSAGE = 0x09,
  CLIENT_TEXT_MESSAGE = 0x0A
};
FW::Net::Protocol_ptr ProtocolApp::create() {
  FW::G::Logger.debug(BOOST_CURRENT_FUNCTION);
  auto protocol = std::make_shared<FW::Net::Protocol>(
      "ProtocolApp", &Private::onConnected, &Private::onPrsePacket);
  return protocol;
}

void ProtocolApp::sendClientPing(FW::Net::Protocol_ptr protocol) {
  static int i = 0;
  i++;

  FW::G::Logger.debug("Send client ping: " + std::to_string(i));

  auto outMsg = std::make_shared<FW::Net::OutputMessage>();
  outMsg->putUInt8(ProtocolCodes::CLIENT_PING);
  protocol->send(outMsg);

  std::this_thread::sleep_for(std::chrono::seconds(1));
}

void ProtocolApp::sendServerPingBack(FW::Net::Protocol_ptr protocol) {
  static int i = 0;
  i++;

  FW::G::Logger.debug("Send server ping back: " + std::to_string(i));

  auto outMsg = std::make_shared<FW::Net::OutputMessage>();
  outMsg->putUInt8(ProtocolCodes::SERVER_PING_BACK);
  protocol->send(outMsg);

  std::this_thread::sleep_for(std::chrono::seconds(1));
}

void ProtocolApp::parseServerTextMessage(FW::Net::Protocol_ptr protocol,
                                         FW::Net::InputMessage& msg) {
  auto message = msg.getString();
  FW::G::Logger.debug("Server text message: " + message);
}
}  // namespace Client

namespace Client::ProtocolApp::Private {
void onConnected(FW::Net::Protocol_ptr& protocol) { sendClientPing(protocol); }
void onPrsePacket(FW::Net::Protocol_ptr& protocol, FW::Net::InputMessage& msg) {
  FW::G::Logger.debug(BOOST_CURRENT_FUNCTION);

  uint8_t packetId = msg.getUInt8();

  try {
    switch (packetId) {
      case ProtocolCodes::SERVER_PING:
        sendServerPingBack(protocol);
        break;
      case ProtocolCodes::CLIENT_PING_BACK:
        sendClientPing(protocol);
        break;
      case ProtocolCodes::SERVER_TEXT_MESSAGE:
        parseServerTextMessage(protocol, msg);
        break;
      default:
        FW::G::Logger.debug("Unknow packet ID: " + std::to_string(packetId));
        break;
    }
  } catch (std::exception e) {
    FW::G::Logger.error("Unable to parse packet[" + std::to_string(packetId) +
                        "]. Error " + std::string(e.what()));
    protocol->disconnect();
  }
}
}  // namespace Client::ProtocolApp::Private
