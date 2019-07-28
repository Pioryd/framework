#include "networkmessage.h"

namespace FW::Net {
NetworkMessage::NetworkMessage()
    : length_{0}, position_{PACKET_SIZE_BYTES_LENGTH} {}

uint16_t NetworkMessage::getLength() const { return length_; }
void NetworkMessage::setLength(uint16_t length) { length_ = length; }

uint8_t* NetworkMessage::getBuffer() { return buffer_; }

const uint8_t* NetworkMessage::getBuffer() const { return buffer_; }

uint8_t* NetworkMessage::getBufferAtCurrentPosition() {
  return buffer_ + position_;
}
const uint8_t* NetworkMessage::getBufferAtCurrentPosition() const {
  return buffer_ + position_;
}

uint16_t NetworkMessage::getBufferPosition() const { return position_; }
}  // namespace FW::Net