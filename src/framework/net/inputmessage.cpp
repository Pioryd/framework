#include "inputmessage.h"

namespace FW::Net {
InputMessage::InputMessage() {}

void InputMessage::reset() {}

uint8_t InputMessage::peekUInt8() { return 0; }

uint16_t InputMessage::peekUInt16() { return 0; }

uint32_t InputMessage::peekUInt32() { return 0; }

uint64_t InputMessage::peekUInt64() { return 0; }

int8_t InputMessage::peekInt8() { return 0; }

int16_t InputMessage::peekInt16() { return 0; }

int32_t InputMessage::peekInt32() { return 0; }

int64_t InputMessage::peekInt64() { return 0; }

std::string InputMessage::peekString() { return ""; }

uint8_t InputMessage::getUInt8() { return 0; }

uint16_t InputMessage::getUInt16() { return 0; }

uint32_t InputMessage::getUInt32() { return 0; }

uint64_t InputMessage::getUInt64() { return 0; }

int8_t InputMessage::getInt8() { return 0; }

int16_t InputMessage::getInt16() { return 0; }

int32_t InputMessage::getInt32() { return 0; }

int64_t InputMessage::getInt64() { return 0; }

std::string InputMessage::getString() { return ""; }

void InputMessage::skipBytes(int16_t count) {}

void InputMessage::throwIfCantRead(size_t size) const {}
}  // namespace FW::Net
