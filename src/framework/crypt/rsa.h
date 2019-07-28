#ifndef FW_CRYPT_RSA_H
#define FW_CRYPT_RSA_H

#include "declarations.h"

#include <cryptopp/rsa.h>

namespace FW::Crypt {
class Rsa {
 public:
  // Key size must be [keySize = (msg.lenght() * 8)]
  Rsa(std::size_t const keySize = 1024);
  ~Rsa();

  void encrypt(unsigned char *msg) const;
  void decrypt(unsigned char *msg) const;

  void generatePrivateAndPublicKey(std::stringstream &ss) const;

  // Must be values in hex format with "0x" on string begin.
  void setPublicKey(const char *n, const char *e);
  // Must be values in hex format with "0x" on string begin.
  void setPrivateKey(const char *n, const char *e, const char *d);

 private:
  std::string getAsString(CryptoPP::RSA::PublicKey const &publicKey) const;
  std::string getAsString(CryptoPP::RSA::PrivateKey const &privateKey) const;

 private:
  std::size_t const keySize;
  std::size_t const msgSize;
  CryptoPP::RSA::PrivateKey privateKey;
  CryptoPP::RSA::PublicKey publicKey;
};
}  // namespace FW::Crypt
#endif  // #ifndef FW_CRYPT_RSA_H