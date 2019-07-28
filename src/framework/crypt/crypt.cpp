#include "crypt.h"

#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

namespace FW::Crypt {

bool Xtea::encrypt(uint8_t* buffer, size_t size, const Key& key) {
  return false;
}

bool Xtea::encrypt(uint16_t* buffer, size_t size, const Key& key) {
  return false;
}

bool Xtea::encrypt(uint32_t* buffer, size_t size, const Key& key) {
  return false;
}

bool Xtea::decrypt(uint8_t* buffer, size_t size, const Key& key) {
  return false;
}

bool Xtea::decrypt(uint16_t* buffer, size_t size, const Key& key) {
  return false;
}

bool Xtea::decrypt(uint32_t* buffer, size_t size, const Key& key) {
  return false;
}

std::string Sha1::hash(const std::string& message) {
  return "";
}

std::string Sha256::hash(const std::string& message) {
  return "";
}

std::string Sha512::hash(const std::string& message) {
  return "";
}
}  // namespace FW::Crypt