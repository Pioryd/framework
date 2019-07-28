#include "rsa.h"

using namespace FW::Crypt;

#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>

Rsa::Rsa(std::size_t const keySize) : keySize(keySize) {}

Rsa::~Rsa() {}

void Rsa::encrypt(unsigned char *msg) const {}

void Rsa::decrypt(unsigned char *msg) const {}

void Rsa::generatePrivateAndPublicKey(std::stringstream &ss) const {}

void Rsa::setPublicKey(const char *n, const char *e) {}

void Rsa::setPrivateKey(const char *n, const char *e, const char *d) {}

std::string Rsa::getAsString(CryptoPP::RSA::PublicKey const &publicKey) const {
  return "";
}

std::string Rsa::getAsString(
    CryptoPP::RSA::PrivateKey const &privateKey) const {
  return "";
}
