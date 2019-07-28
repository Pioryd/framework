#ifndef FW_CRYPT_H
#define FW_CRYPT_H

#include "declarations.h"

namespace FW::Crypt {
namespace Xtea {
using Key = std::array<uint32_t, 4>;
bool encrypt(uint8_t* buffer, size_t size, const Key& key);
bool encrypt(uint16_t* buffer, size_t size, const Key& key);
bool encrypt(uint32_t* buffer, size_t size, const Key& key);
bool decrypt(uint8_t* buffer, size_t size, const Key& key);
bool decrypt(uint16_t* buffer, size_t size, const Key& key);
bool decrypt(uint32_t* buffer, size_t size, const Key& key);
}  // namespace Xtea
namespace Sha1 {
std::string hash(const std::string& message);
}  // namespace Sha1
namespace Sha256 {
std::string hash(const std::string& message);
}  // namespace Sha256
namespace Sha512 {
std::string hash(const std::string& message);
}  // namespace Sha512
}  // namespace FW::Crypt
#endif  // #ifndef FW_CRYPT_H