#ifndef FW_NET_NETWORKMESSAGE_H
#define FW_NET_NETWORKMESSAGE_H

#include "declarations.h"

namespace FW::Net {
class NetworkMessage {
 public:
  static constexpr uint8_t PADDING_BYTE{0x64};
  static constexpr uint32_t BUFFER_MAXSIZE{65536};

  static constexpr uint32_t PACKET_SIZE_BYTES_LENGTH{2};

  static constexpr uint32_t MAX_PACKET_BODY_LENGTH =
      BUFFER_MAXSIZE - PACKET_SIZE_BYTES_LENGTH;

 protected:
  NetworkMessage();

 public:
  virtual ~NetworkMessage() = default;

 public:
  virtual void reset() = 0;

  uint16_t getLength() const;
  void setLength(uint16_t newLength);

  uint8_t* getBuffer();
  const uint8_t* getBuffer() const;

  uint8_t* getBufferAtCurrentPosition();
  const uint8_t* getBufferAtCurrentPosition() const;

  uint16_t getBufferPosition() const;

 protected:
  uint16_t length_;
  uint16_t position_;
  uint8_t buffer_[BUFFER_MAXSIZE];
};
}  // namespace FW::Net
#endif  // #ifndef FW_NET_NETWORKMESSAGE_H