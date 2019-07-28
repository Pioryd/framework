#ifndef FW_UTIL_MATH_H
#define FW_UTIL_MATH_H

#include "declarations.h"

namespace FW::Util {
namespace Math {
template <typename T>
inline T readLE8(const uint8_t* data) {
  return data[0];
}
template <typename T>
inline T readLE16(const uint8_t* data) {
  return ((data[1] << 8) | data[0]);
}
template <typename T>
inline T readLE32(const uint8_t* data) {
  return (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | (data[0]);
}
// int64/uin64 is still only 40bits at x86-64
// be sure you are in correct range of min/max
template <typename T>
inline T readLE64(const uint8_t* data) {
  return ((T)data[7] << 56) | ((T)data[6] << 48) | ((T)data[5] << 40) |
         ((T)data[4] << 32) | ((T)data[3] << 24) | ((T)data[2] << 16) |
         ((T)data[1] << 8) | ((T)data[0]);
}

template <typename T>
inline T readBE8(const uint8_t* data) {
  return data[0];
}
template <typename T>
inline T readBE16(const uint8_t* data) {
  return ((data[0] << 8) | data[1]);
}
template <typename T>
inline T readBE32(const uint8_t* data) {
  return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3]);
}
// int64/uin64 is still only 40bits at x86-64
// be sure you are in correct range of min/max
template <typename T>
inline T readBE64(const uint8_t* data) {
  return ((T)data[0] << 56) | ((T)data[1] << 48) | ((T)data[2] << 40) |
         ((T)data[3] << 32) | ((T)data[4] << 24) | ((T)data[5] << 16) |
         ((T)data[6] << 8) | ((T)data[7]);
}

template <typename T>
inline void writeLE8(uint8_t* data, T value) {
  data[0] = (T)value;
}
template <typename T>
inline void writeLE16(uint8_t* data, T value) {
  data[1] = ((T)value >> 8) & 0xFF;
  data[0] = (T)value;
}
template <typename T>
inline void writeLE32(uint8_t* data, T value) {
  data[3] = ((T)value >> 24) & 0xFF;
  data[2] = ((T)value >> 16) & 0xFF;
  data[1] = ((T)value >> 8) & 0xFF;
  data[0] = (T)value;
}
template <typename T>
inline void writeLE64(uint8_t* data, T value) {
  data[7] = ((T)value >> 56) & 0xFF;
  data[6] = ((T)value >> 48) & 0xFF;
  data[5] = ((T)value >> 40) & 0xFF;
  data[4] = ((T)value >> 32) & 0xFF;
  data[3] = ((T)value >> 24) & 0xFF;
  data[2] = ((T)value >> 16) & 0xFF;
  data[1] = ((T)value >> 8) & 0xFF;
  data[0] = (T)value;
}

template <typename T>
inline void writeBE8(uint8_t* data, T value) {
  data[0] = (T)value;
}
template <typename T>
inline void writeBE16(uint8_t* data, T value) {
  data[0] = ((T)value >> 8) & 0xFF;
  data[1] = (T)value;
}
template <typename T>
inline void writeBE32(uint8_t* data, T value) {
  data[0] = ((T)value >> 24) & 0xFF;
  data[1] = ((T)value >> 16) & 0xFF;
  data[2] = ((T)value >> 8) & 0xFF;
  data[3] = (T)value;
}
template <typename T>
inline void writeBE64(uint8_t* data, T value) {
  data[0] = ((T)value >> 56) & 0xFF;
  data[1] = ((T)value >> 48) & 0xFF;
  data[2] = ((T)value >> 40) & 0xFF;
  data[3] = ((T)value >> 32) & 0xFF;
  data[4] = ((T)value >> 24) & 0xFF;
  data[5] = ((T)value >> 16) & 0xFF;
  data[6] = ((T)value >> 8) & 0xFF;
  data[7] = (T)value;
}

uint32_t adler32(const uint8_t* data, size_t lenght);

long random_range(long min, long max);
float random_range(float min, float max);

inline bool is_power_of_two(size_t v) { return ((v != 0) && !(v & (v - 1))); }
inline size_t to_power_of_two(size_t v) {
  if (v == 0) return 0;
  size_t r = 1;
  while (r < v && r != 0xffffffff) r <<= 1;
  return r;
}

template <typename T>
T clamp(T x, T min, T max) {
  return std::max<T>(min, std::min<T>(x, max));
}
}  // namespace Math
}  // namespace FW::Util
#endif  // FW_UTIL_MATH_H
