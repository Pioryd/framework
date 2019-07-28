#include "outputmessage.h"

namespace FW::Net {
OutputMessage::OutputMessage() : NetworkMessage() {}

void OutputMessage::reset() {}

void OutputMessage::putUInt8(uint8_t value) {}

void OutputMessage::putUInt16(uint16_t value) {}

void OutputMessage::putUInt32(uint32_t value) {}

void OutputMessage::putUInt64(uint64_t value) {}

void OutputMessage::putInt8(int8_t value) {}

void OutputMessage::putInt16(int16_t value) {}

void OutputMessage::putInt32(int32_t value) {}

void OutputMessage::putInt64(int64_t value) {}

void OutputMessage::addString(const std::string& value) {}

void OutputMessage::putBytes(const char* bytes, size_t size) {}

void OutputMessage::addPaddingBytes(size_t count) {}

void OutputMessage::throwIfCantPut(size_t size) const {}
}  // namespace FW::Net
