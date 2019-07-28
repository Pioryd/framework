#include "networkmessage.h"

namespace FW::Net {
NetworkMessage::NetworkMessage() {}

uint16_t NetworkMessage::getLength() const { return 0; }
void NetworkMessage::setLength(uint16_t length) {}

uint8_t* NetworkMessage::getBuffer() { return 0; }

const uint8_t* NetworkMessage::getBuffer() const { return 0; }

uint8_t* NetworkMessage::getBufferAtCurrentPosition() { return 0; }

const uint8_t* NetworkMessage::getBufferAtCurrentPosition() const { return 0; }

uint16_t NetworkMessage::getBufferPosition() const { return 0; }
}  // namespace FW::Net