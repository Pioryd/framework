#include "crypt.h"

#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

namespace FW::Crypt {

bool Xtea::encrypt(uint8_t* buffer, size_t size, const Key& key) {
  return encrypt(reinterpret_cast<uint32_t*>(buffer),
                 size / (sizeof(uint32_t) / sizeof(uint8_t)), key);
}

bool Xtea::encrypt(uint16_t* buffer, size_t size, const Key& key) {
  return encrypt(reinterpret_cast<uint32_t*>(buffer),
                 size / (sizeof(uint32_t) / sizeof(uint16_t)), key);
}

bool Xtea::encrypt(uint32_t* buffer, size_t size, const Key& key) {
  const unsigned int num_rounds = 32;
  int read_pos = 0;

  while (read_pos < size) {
    uint32_t v0 = buffer[read_pos], v1 = buffer[read_pos + 1];
    uint32_t delta = 0x9E3779B9;
    uint32_t sum = 0;

    for (auto i = 0; i < num_rounds; i++) {
      v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
      sum += delta;
      v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
    }

    buffer[read_pos] = v0;
    buffer[read_pos + 1] = v1;
    read_pos = read_pos + 2;
  }

  return true;
}

bool Xtea::decrypt(uint8_t* buffer, size_t size, const Key& key) {
  return decrypt(reinterpret_cast<uint32_t*>(buffer),
                 size / (sizeof(uint32_t) / sizeof(uint8_t)), key);
}

bool Xtea::decrypt(uint16_t* buffer, size_t size, const Key& key) {
  return decrypt(reinterpret_cast<uint32_t*>(buffer),
                 size / (sizeof(uint32_t) / sizeof(uint16_t)), key);
}

bool Xtea::decrypt(uint32_t* buffer, size_t size, const Key& key) {
  if (size % ((sizeof(uint32_t) * 8)) != 0) {
    throw std::runtime_error("Invalid encrypted buffer");
    return false;
  }

  const unsigned int num_rounds = 32;
  int read_pos = 0;

  while (read_pos < size) {
    uint32_t v0 = buffer[read_pos], v1 = buffer[read_pos + 1];
    uint32_t delta = 0x9E3779B9;

    uint32_t sum = delta * num_rounds;
    unsigned int i;

    for (i = 0; i < num_rounds; i++) {
      v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
      sum -= delta;
      v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }

    buffer[read_pos] = v0;
    buffer[read_pos + 1] = v1;
    read_pos = read_pos + 2;
  }

  return true;
}

std::string Sha1::hash(const std::string& message) {
  CryptoPP::SHA1 hash;
  std::string digest;

  CryptoPP::StringSource s(
      message, true,
      new CryptoPP::HashFilter(
          hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));

  std::transform(digest.begin(), digest.end(), digest.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return digest;
}

std::string Sha256::hash(const std::string& message) {
  CryptoPP::SHA256 hash;
  std::string digest;

  CryptoPP::StringSource s(
      message, true,
      new CryptoPP::HashFilter(
          hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));

  std::transform(digest.begin(), digest.end(), digest.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return digest;
}

std::string Sha512::hash(const std::string& message) {
  CryptoPP::SHA512 hash;
  std::string digest;

  CryptoPP::StringSource s(
      message, true,
      new CryptoPP::HashFilter(
          hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));

  std::transform(digest.begin(), digest.end(), digest.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return digest;
}
}  // namespace FW::Crypt