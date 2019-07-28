#include "rsa.h"

using namespace FW::Crypt;

#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>

static CryptoPP::AutoSeededRandomPool prng;

Rsa::Rsa(std::size_t const keySize) : keySize(keySize), msgSize(keySize / 8) {}

Rsa::~Rsa() {}

void Rsa::encrypt(unsigned char *msg) const {
  CryptoPP::Integer m{msg, msgSize};
  CryptoPP::Integer enc = publicKey.ApplyFunction(m);
  enc.Encode(msg, msgSize);
}

void Rsa::decrypt(unsigned char *msg) const {
  CryptoPP::Integer m{msg, msgSize};
  auto ci = privateKey.CalculateInverse(prng, m);
  ci.Encode(msg, msgSize);
}

void Rsa::generatePrivateAndPublicKey(std::stringstream &ss) const {
  CryptoPP::AutoSeededRandomPool rng;
  CryptoPP::InvertibleRSAFunction params;

  params.GenerateRandomWithKeySize(rng, this->keySize);

  CryptoPP::RSA::PublicKey genPublicKey = CryptoPP::RSA::PublicKey(params);
  CryptoPP::RSA::PrivateKey genPrivateKey = CryptoPP::RSA::PrivateKey(params);

  CryptoPP::Integer const &n = params.GetModulus();
  CryptoPP::Integer const &p = params.GetPrime1();
  CryptoPP::Integer const &q = params.GetPrime2();
  CryptoPP::Integer const &d = params.GetPrivateExponent();
  CryptoPP::Integer const &e = params.GetPublicExponent();

  ss << "Public key values:" << std::endl
     << " (HEX) n: " << std::hex << n << std::endl
     << " n: " << std::dec << n << std::endl
     << " (HEX) e: " << std::hex << e << std::endl
     << " e: " << std::dec << e << std::endl
     << " public PEM: " << std::endl
     << "-----BEGIN PUBLIC KEY-----" << std::endl
     << getAsString(genPublicKey) << "-----END PUBLIC KEY-----" << std::endl;

  ss << "Private key values:" << std::endl
     << " (HEX) p: " << std::hex << p << std::endl
     << " p: " << std::dec << p << std::endl
     << " (HEX) q: " << std::hex << q << std::endl
     << " q: " << std::dec << q << std::endl
     << " (HEX) d: " << std::hex << d << std::endl
     << " d: " << std::dec << d << std::endl
     << " private PEM: " << std::endl
     << "-----BEGIN RSA PRIVATE KEY-----" << std::endl
     << getAsString(genPrivateKey) << "-----END RSA PRIVATE KEY-----"
     << std::endl;
}

void Rsa::setPublicKey(const char *n, const char *e) {
  CryptoPP::Integer iN{n};
  CryptoPP::Integer iE{e};
  publicKey.Initialize(iN, iE);
}

void Rsa::setPrivateKey(const char *n, const char *e, const char *d) {
  CryptoPP::Integer iN{n};
  CryptoPP::Integer iE{e};
  CryptoPP::Integer iD{d};

  privateKey.Initialize(iN, iE, iD);
}

std::string Rsa::getAsString(CryptoPP::RSA::PublicKey const &publicKey) const {
  std::string stringKey;
  CryptoPP::Base64Encoder keySink;

  publicKey.DEREncode(keySink);
  keySink.MessageEnd();

  auto size = keySink.MaxRetrievable();
  if (size) {
    stringKey.resize(size);
    keySink.Get((unsigned char *)&stringKey[0], stringKey.size());
  }

  return stringKey;
}

std::string Rsa::getAsString(
    CryptoPP::RSA::PrivateKey const &privateKey) const {
  std::string stringKey;
  CryptoPP::Base64Encoder keySink;

  privateKey.DEREncode(keySink);
  keySink.MessageEnd();

  auto size = keySink.MaxRetrievable();
  if (size) {
    stringKey.resize(size);
    keySink.Get((unsigned char *)&stringKey[0], stringKey.size());
  }

  return stringKey;
}
