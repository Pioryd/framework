#include "inputmessage.h"

#include "../global.h"
#include "../util/math.h"

namespace FW::Net {
InputMessage::InputMessage() { position_ = 0; }

void InputMessage::reset() {
  length_ = 0;
  position_ = 0;
}

uint8_t InputMessage::peek_uint8() {
  throw_if_cant_read(1);
  return Util::Math::read_le8<uint8_t>(buffer_ + position_);
}

uint16_t InputMessage::peek_uint16() {
  throw_if_cant_read(2);
  return Util::Math::read_le16<uint16_t>(buffer_ + position_);
}

uint32_t InputMessage::peek_uint32() {
  throw_if_cant_read(4);
  return Util::Math::read_le32<uint32_t>(buffer_ + position_);
}

uint64_t InputMessage::peek_uint64() {
  throw_if_cant_read(8);
  return Util::Math::read_le64<uint64_t>(buffer_ + position_);
}

int8_t InputMessage::peek_int8() {
  throw_if_cant_read(1);
  return Util::Math::read_le8<uint8_t>(buffer_ + position_);
}

int16_t InputMessage::peek_int16() {
  throw_if_cant_read(2);
  return Util::Math::read_le16<uint16_t>(buffer_ + position_);
}

int32_t InputMessage::peek_int32() {
  throw_if_cant_read(4);
  return Util::Math::read_le32<uint32_t>(buffer_ + position_);
}

int64_t InputMessage::peek_int64() {
  throw_if_cant_read(8);
  return Util::Math::read_le64<uint64_t>(buffer_ + position_);
}

std::string InputMessage::peek_string() {
  uint16_t string_length = peek_uint16();
  throw_if_cant_read(string_length);
  char* value = reinterpret_cast<char*>(buffer_) + position_;
  return std::string(value, string_length);
}

uint8_t InputMessage::get_uint8() {
  uint8_t value = peek_uint8();
  position_ += 1;
  return value;
}

uint16_t InputMessage::get_uint16() {
  uint16_t value = peek_uint16();
  position_ += 2;
  return value;
}

uint32_t InputMessage::get_uint32() {
  uint32_t value = peek_uint32();
  position_ += 4;
  return value;
}

uint64_t InputMessage::get_uint64() {
  uint64_t value = peek_uint64();
  position_ += 8;
  return value;
}

int8_t InputMessage::get_int8() {
  uint8_t value = peek_int8();
  position_ += 1;
  return value;
}

int16_t InputMessage::get_int16() {
  uint16_t value = peek_int16();
  position_ += 2;
  return value;
}

int32_t InputMessage::get_int32() {
  uint32_t value = peek_int32();
  position_ += 4;
  return value;
}

int64_t InputMessage::get_int64() {
  uint64_t value = peek_int64();
  position_ += 8;
  return value;
}

std::string InputMessage::get_string() {
  uint16_t string_length = get_uint16();
  throw_if_cant_read(string_length);
  char* value = reinterpret_cast<char*>(buffer_) + position_;
  position_ += string_length;
  return std::string(value, string_length);
}

void InputMessage::skip_bytes(int16_t count) { position_ += count; }

void InputMessage::throw_if_cant_read(size_t size) const {
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
