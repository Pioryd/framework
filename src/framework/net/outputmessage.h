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

  void put_uint8(uint8_t value);
  void put_uint16(uint16_t value);
  void put_uint32(uint32_t value);
  void put_uint64(uint64_t value);

  void put_int8(int8_t value);
  void put_int16(int16_t value);
  void put_int32(int32_t value);
  void put_int64(int64_t value);

  void add_string(const std::string& value);
  void put_bytes(const char* bytes, size_t size);
  void add_padding_bytes(size_t count);

  uint8_t* get_output_buffer() { return buffer_ + output_buffer_start_; }
  void write_message_length() { add_header(length_); }

  void append(const OutputMessage& msg) {
    auto msg_len = msg.get_length();
    memcpy(buffer_ + position_, msg.get_buffer() + 8, msg_len);
    length_ += msg_len;
    position_ += msg_len;
  }

 private:
  void throw_if_cant_put(size_t size) const;

  template <typename T>
  void add_header(T add) {
    assert(output_buffer_start_ >= sizeof(T));
    output_buffer_start_ -= sizeof(T);
    memcpy(buffer_ + output_buffer_start_, &add, sizeof(T));
    // added header size to the message size
    length_ += sizeof(T);
  }

  uint16_t output_buffer_start_ = PACKET_SIZE_BYTES_LENGTH;
};
}  // namespace FW::Net
#endif  // #ifndef FW_NET_OUTPUTMESSAGE_H
