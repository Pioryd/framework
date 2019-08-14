#include "networkmessage.h"

namespace FW::Net {
NetworkMessage::NetworkMessage()
    : length_{0}, position_{PACKET_SIZE_BYTES_LENGTH} {}

uint16_t NetworkMessage::get_length() const { return length_; }
void NetworkMessage::set_length(uint16_t length) { length_ = length; }

uint8_t* NetworkMessage::get_buffer() { return buffer_; }

const uint8_t* NetworkMessage::get_buffer() const { return buffer_; }

uint8_t* NetworkMessage::get_buffer_at_current_position() {
  return buffer_ + position_;
}
const uint8_t* NetworkMessage::get_buffer_at_current_position() const {
  return buffer_ + position_;
}

uint16_t NetworkMessage::get_buffer_position() const { return position_; }
}  // namespace FW::Net