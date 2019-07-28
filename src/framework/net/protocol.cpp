#include "protocol.h"

namespace FW::Net {
Protocol::Protocol(
    const std::string& name, std::function<void(Protocol_ptr&)> onConnected,
    std::function<void(Protocol_ptr&, InputMessage&)> onPrsePacket) {}

void Protocol::setConnection(const Connection_ptr& connection) {}

void Protocol::prepareToWrite(const OutputMessage_ptr& msg) const {}

void Protocol::onRecvMessage(InputMessage& msg) {}

Connection_ptr Protocol::getConnection() const { return nullptr; }

void Protocol::send(const OutputMessage_ptr& msg) const {}

void Protocol::disconnect() const {}
}  // namespace FW::Net
