#ifndef FW_NET_INPUTMESSAGE_H
#define FW_NET_INPUTMESSAGE_H

#include "networkmessage.h"

namespace FW::Net {
class InputMessage : public NetworkMessage {
 public:
  InputMessage();

  InputMessage(const InputMessage&) = delete;
  InputMessage& operator=(const InputMessage&) = delete;

  void reset() override;

  uint8_t peek_uint8();
  uint16_t peek_uint16();
  uint32_t peek_uint32();
  uint64_t peek_uint64();

  int8_t peek_int8();
  int16_t peek_int16();
  int32_t peek_int32();
  int64_t peek_int64();

  std::string peek_string();

  uint8_t get_uint8();
  uint16_t get_uint16();
  uint32_t get_uint32();
  uint64_t get_uint64();

  int8_t get_int8();
  int16_t get_int16();
  int32_t get_int32();
  int64_t get_int64();

  std::string get_string();

  void skip_bytes(int16_t count);

 protected:
  void throw_if_cant_read(size_t size) const;
};
}  // namespace FW::Net
#endif  // #ifndef FW_NET_INPUTMESSAGE_H
