#include "outputmessage.h"

namespace FW::Net {
OutputMessage::OutputMessage() : NetworkMessage() {}

void OutputMessage::reset() {
  length_ = 0;
  position_ = PACKET_SIZE_BYTES_LENGTH;
}

void OutputMessage::putUInt8(uint8_t value) {
  throwIfCantPut(1);
  Util::Math::writeLE8<uint8_t>(buffer_ + position_, value);
  position_ += 1;
  length_ += 1;
}

void OutputMessage::putUInt16(uint16_t value) {
  throwIfCantPut(2);
  Util::Math::writeLE16<uint16_t>(buffer_ + position_, value);
  position_ += 2;
  length_ += 2;
}

void OutputMessage::putUInt32(uint32_t value) {
  throwIfCantPut(4);
  Util::Math::writeLE32<uint32_t>(buffer_ + position_, value);
  position_ += 4;
  length_ += 4;
}

void OutputMessage::putUInt64(uint64_t value) {
  throwIfCantPut(8);
  Util::Math::writeLE64<uint64_t>(buffer_ + position_, value);
  position_ += 8;
  length_ += 8;
}

void OutputMessage::putInt8(int8_t value) {
  throwIfCantPut(1);
  Util::Math::writeLE8<int8_t>(buffer_ + position_, value);
  position_ += 1;
  length_ += 1;
}

void OutputMessage::putInt16(int16_t value) {
  throwIfCantPut(2);
  Util::Math::writeLE16<int16_t>(buffer_ + position_, value);
  position_ += 2;
  length_ += 2;
}

void OutputMessage::putInt32(int32_t value) {
  throwIfCantPut(2);
  Util::Math::writeLE32<int32_t>(buffer_ + position_, value);
  position_ += 4;
  length_ += 4;
}

void OutputMessage::putInt64(int64_t value) {
  throwIfCantPut(2);
  Util::Math::writeLE64<int64_t>(buffer_ + position_, value);
  position_ += 8;
  length_ += 8;
}

void OutputMessage::addString(const std::string& value) {
  size_t stringLen = value.length();
  throwIfCantPut(stringLen);

  putUInt16(stringLen);
  memcpy(buffer_ + position_, value.c_str(), stringLen);
  position_ += stringLen;
  length_ += stringLen;
}

void OutputMessage::putBytes(const char* bytes, size_t size) {
  throwIfCantPut(size);
  memcpy(buffer_ + position_, bytes, size);
  position_ += size;
  length_ += size;
}

void OutputMessage::addPaddingBytes(size_t count) {
  throwIfCantPut(count);
  memset(buffer_ + position_, PADDING_BYTE, count);
  length_ += count;
}

void OutputMessage::throwIfCantPut(size_t size) const {
  if ((size + position_) >= MAX_PACKET_BODY_LENGTH)
    throw std::runtime_error(
        std::string("Unable to put bytes. Out of buffer_ [" +
                    std::to_string(size + position_) + "/" +
                    std::to_string(MAX_PACKET_BODY_LENGTH) + "].")
            .c_str());
}
}  // namespace FW::Net
