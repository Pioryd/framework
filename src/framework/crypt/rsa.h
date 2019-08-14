#ifndef FW_CRYPT_RSA_H
#define FW_CRYPT_RSA_H

#include "declarations.h"

#include <cryptopp/rsa.h>

namespace FW::Crypt {
class Rsa {
 public:
  // Key size must be [keySize = (msg.lenght() * 8)]
  Rsa(std::size_t const key_size = 1024);
  ~Rsa();

  void encrypt(unsigned char *msg) const;
  void decrypt(unsigned char *msg) const;

  void generate_private_and_public_key(std::stringstream &ss) const;

  // Must be values in hex format with "0x" on string begin.
  void set_public_key(const char *n, const char *e);
  // Must be values in hex format with "0x" on string begin.
  void set_private_key(const char *n, const char *e, const char *d);

 private:
  std::size_t const key_size_;
  std::size_t const msg_size_;
  CryptoPP::RSA::PrivateKey private_key_;
  CryptoPP::RSA::PublicKey public_key_;
};
}  // namespace FW::Crypt
#endif  // #ifndef FW_CRYPT_RSA_H