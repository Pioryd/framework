#include "math.h"

#include <random>

namespace FW::Util {
uint32_t Math::adler32(const uint8_t* data, size_t lenght) {
  constexpr uint32_t adler = 65521;
  uint32_t a = 1, b = 0;
  size_t tLenght;

  while (lenght > 0) {
    tLenght = lenght > 5552 ? 5552 : lenght;
    lenght -= tLenght;
    do {
      a += *data++;
      b += a;
    } while (--tLenght);

    a %= adler;
    b %= adler;
  }
  return (b << 16) | a;
}

long Math::random_range(long min, long max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<long> dis(0, 2147483647);
  return min + (dis(gen) % (max - min + 1));
}

float Math::random_range(float min, float max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<float> dis(0.0, 1.0);
  return min + (max - min) * dis(gen);
}
}  // namespace FW::Util