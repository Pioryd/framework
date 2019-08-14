#include "outputmessage.h"

namespace FW::Net {
OutputMessage::OutputMessage() : NetworkMessage() {}

void OutputMessage::reset() {
  length_ = 0;
  position_ = PACKET_SIZE_BYTES_LENGTH;
}

void OutputMessage::put_uint8(uint8_t value) {
  throw_if_cant_put(1);
  Util::Math::write_le8<uint8_t>(buffer_ + position_, value);
  position_ += 1;
  length_ += 1;
}

void OutputMessage::put_uint16(uint16_t value) {
  throw_if_cant_put(2);
  Util::Math::write_le16<uint16_t>(buffer_ + position_, value);
  position_ += 2;
  length_ += 2;
}

void OutputMessage::put_uint32(uint32_t value) {
  throw_if_cant_put(4);
  Util::Math::write_le32<uint32_t>(buffer_ + position_, value);
  position_ += 4;
  length_ += 4;
}

void OutputMessage::put_uint64(uint64_t value) {
  throw_if_cant_put(8);
  Util::Math::write_le64<uint64_t>(buffer_ + position_, value);
  position_ += 8;
  length_ += 8;
}

void OutputMessage::put_int8(int8_t value) {
  throw_if_cant_put(1);
  Util::Math::write_le8<int8_t>(buffer_ + position_, value);
  position_ += 1;
  length_ += 1;
}

void OutputMessage::put_int16(int16_t value) {
  throw_if_cant_put(2);
  Util::Math::write_le16<int16_t>(buffer_ + position_, value);
  position_ += 2;
  length_ += 2;
}

void OutputMessage::put_int32(int32_t value) {
  throw_if_cant_put(2);
  Util::Math::write_le32<int32_t>(buffer_ + position_, value);
  position_ += 4;
  length_ += 4;
}

void OutputMessage::put_int64(int64_t value) {
  throw_if_cant_put(2);
  Util::Math::write_le64<int64_t>(buffer_ + position_, value);
  position_ += 8;
  length_ += 8;
}

void OutputMessage::add_string(const std::string& value) {
  size_t string_len = value.length();
  throw_if_cant_put(string_len);

  put_uint16(string_len);
  memcpy(buffer_ + position_, value.c_str(), string_len);
  position_ += string_len;
  length_ += string_len;
}

void OutputMessage::put_bytes(const char* bytes, size_t size) {
  throw_if_cant_put(size);
  memcpy(buffer_ + position_, bytes, size);
  position_ += size;
  length_ += size;
}

void OutputMessage::add_padding_bytes(size_t count) {
  throw_if_cant_put(count);
  memset(buffer_ + position_, PADDING_BYTE, count);
  length_ += count;
}

void OutputMessage::throw_if_cant_put(size_t size) const {
  if ((size + position_) >= MAX_PACKET_BODY_LENGTH)
    throw std::runtime_error(
        std::string("Unable to put bytes. Out of buffer_ [" +
                    std::to_string(size + position_) + "/" +
                    std::to_string(MAX_PACKET_BODY_LENGTH) + "].")
            .c_str());
}
}  // namespace FW::Net
