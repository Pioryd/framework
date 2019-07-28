#ifndef FW_NET_OUTPUTMESSAGE_H
#define FW_NET_OUTPUTMESSAGE_H

#include "../crypt/crypt.h"
#include "../util/math.h"
#include "networkmessage.h"

#include <cstring>

namespace FW::Net {
class OutputMessage : public NetworkMessage {
 public:
  static constexpr uint32_t MAX_STRING_LENGTH = 65536;

 public:
  OutputMessage();

  OutputMessage(const OutputMessage&) = delete;
  OutputMessage& operator=(const OutputMessage&) = delete;

  void reset() override;

  void putUInt8(uint8_t value);
  void putUInt16(uint16_t value);
  void putUInt32(uint32_t value);
  void putUInt64(uint64_t value);

  void putInt8(int8_t value);
  void putInt16(int16_t value);
  void putInt32(int32_t value);
  void putInt64(int64_t value);

  void addString(const std::string& value);
  void putBytes(const char* bytes, size_t size);
  void addPaddingBytes(size_t count);

  uint8_t* getOutputBuffer() { return buffer_ + outputBufferStart; }
  void writeMessageLength() { add_header(length_); }

  void append(const OutputMessage& msg) {
    auto msgLen = msg.getLength();
    memcpy(buffer_ + position_, msg.getBuffer() + 8, msgLen);
    length_ += msgLen;
    position_ += msgLen;
  }

 private:
  void throwIfCantPut(size_t size) const;

  template <typename T>
  void add_header(T add) {
    assert(outputBufferStart >= sizeof(T));
    outputBufferStart -= sizeof(T);
    memcpy(buffer_ + outputBufferStart, &add, sizeof(T));
    // added header size to the message size
    length_ += sizeof(T);
  }

  uint16_t outputBufferStart = PACKET_SIZE_BYTES_LENGTH;
};
}  // namespace FW::Net
#endif  // #ifndef FW_NET_OUTPUTMESSAGE_H
