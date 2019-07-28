#include "protocol.h"

#include "../global.h"
#include "../thread/eventmanager.h"
#include "outputmessage.h"

const uint16_t OUTPUTMESSAGE_FREE_LIST_CAPACITY = 2048;
const std::chrono::milliseconds OUTPUTMESSAGE_AUTOSEND_DELAY{10};

namespace FW::Net {
Protocol::Protocol(
    const std::string& name, std::function<void(Protocol_ptr&)> onConnected,
    std::function<void(Protocol_ptr&, InputMessage&)> onPrsePacket)
    : name_{name}, onConnected{onConnected}, onPrsePacket{onPrsePacket} {}

void Protocol::setConnection(const Connection_ptr& connection) {
  connection_ = connection;
}

void Protocol::prepareToWrite(const OutputMessage_ptr& msg) const {
  msg->writeMessageLength();
}

void Protocol::onRecvMessage(InputMessage& msg) {
  auto this_ptr = shared_from_this();
  onPrsePacket(this_ptr, msg);
}

Connection_ptr Protocol::getConnection() const { return connection_.lock(); }

void Protocol::send(const OutputMessage_ptr& msg) const {
  if (auto connection = getConnection()) { connection->send(msg); }
}

void Protocol::disconnect() const {
  if (auto connection = getConnection()) { connection->close(); }
}
}  // namespace FW::Net
