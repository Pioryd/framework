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

  uint8_t peekUInt8();
  uint16_t peekUInt16();
  uint32_t peekUInt32();
  uint64_t peekUInt64();

  int8_t peekInt8();
  int16_t peekInt16();
  int32_t peekInt32();
  int64_t peekInt64();

  std::string peekString();

  uint8_t getUInt8();
  uint16_t getUInt16();
  uint32_t getUInt32();
  uint64_t getUInt64();

  int8_t getInt8();
  int16_t getInt16();
  int32_t getInt32();
  int64_t getInt64();

  std::string get_string();

  void skipBytes(int16_t count);

 protected:
  void throwIfCantRead(size_t size) const;
};
}  // namespace FW::Net
#endif  // #ifndef FW_NET_INPUTMESSAGE_H
