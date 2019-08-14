#include "inputmessage.h"

#include "../global.h"
#include "../util/math.h"

namespace FW::Net {
InputMessage::InputMessage() { position_ = 0; }

void InputMessage::reset() {
  length_ = 0;
  position_ = 0;
}

uint8_t InputMessage::peekUInt8() {
  throwIfCantRead(1);
  return Util::Math::read_le8<uint8_t>(buffer_ + position_);
}

uint16_t InputMessage::peekUInt16() {
  throwIfCantRead(2);
  return Util::Math::read_le16<uint16_t>(buffer_ + position_);
}

uint32_t InputMessage::peekUInt32() {
  throwIfCantRead(4);
  return Util::Math::read_le32<uint32_t>(buffer_ + position_);
}

uint64_t InputMessage::peekUInt64() {
  throwIfCantRead(8);
  return Util::Math::read_le64<uint64_t>(buffer_ + position_);
}

int8_t InputMessage::peekInt8() {
  throwIfCantRead(1);
  return Util::Math::read_le8<uint8_t>(buffer_ + position_);
}

int16_t InputMessage::peekInt16() {
  throwIfCantRead(2);
  return Util::Math::read_le16<uint16_t>(buffer_ + position_);
}

int32_t InputMessage::peekInt32() {
  throwIfCantRead(4);
  return Util::Math::read_le32<uint32_t>(buffer_ + position_);
}

int64_t InputMessage::peekInt64() {
  throwIfCantRead(8);
  return Util::Math::read_le64<uint64_t>(buffer_ + position_);
}

std::string InputMessage::peekString() {
  uint16_t stringLength = peekUInt16();
  throwIfCantRead(stringLength);
  char* value = reinterpret_cast<char*>(buffer_) + position_;
  return std::string(value, stringLength);
}

uint8_t InputMessage::getUInt8() {
  uint8_t value = peekUInt8();
  position_ += 1;
  return value;
}

uint16_t InputMessage::getUInt16() {
  uint16_t value = peekUInt16();
  position_ += 2;
  return value;
}

uint32_t InputMessage::getUInt32() {
  uint32_t value = peekUInt32();
  position_ += 4;
  return value;
}

uint64_t InputMessage::getUInt64() {
  uint64_t value = peekUInt64();
  position_ += 8;
  return value;
}

int8_t InputMessage::getInt8() {
  uint8_t value = peekInt8();
  position_ += 1;
  return value;
}

int16_t InputMessage::getInt16() {
  uint16_t value = peekInt16();
  position_ += 2;
  return value;
}

int32_t InputMessage::getInt32() {
  uint32_t value = peekInt32();
  position_ += 4;
  return value;
}

int64_t InputMessage::getInt64() {
  uint64_t value = peekInt64();
  position_ += 8;
  return value;
}

std::string InputMessage::getString() {
  uint16_t stringLength = getUInt16();
  throwIfCantRead(stringLength);
  char* value = reinterpret_cast<char*>(buffer_) + position_;
  position_ += stringLength;
  return std::string(value, stringLength);
}

void InputMessage::skipBytes(int16_t count) { position_ += count; }

void InputMessage::throwIfCantRead(size_t size) const {
  if ((position_ + size) >= (length_ + PACKET_SIZE_BYTES_LENGTH) ||
      size >= (BUFFER_MAXSIZE - position_)) {
    throw std::runtime_error(
        std::string("Unable to read bytes. Out of buffer_ [" +
                    std::to_string(size + position_) + "/" +
                    std::to_string(MAX_PACKET_BODY_LENGTH) + "].")
            .c_str());
  }
}
}  // namespace FW::Net
