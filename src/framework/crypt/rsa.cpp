#include "rsa.h"

using namespace FW::Crypt;

#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>

static CryptoPP::AutoSeededRandomPool prng;

Rsa::Rsa(std::size_t const key_size)
    : key_size_(key_size), msg_size_(key_size / 8) {}

Rsa::~Rsa() {}

void Rsa::encrypt(unsigned char *msg) const {
  CryptoPP::Integer m{msg, msg_size_};
  CryptoPP::Integer enc = public_key_.ApplyFunction(m);
  enc.Encode(msg, msg_size_);
}

void Rsa::decrypt(unsigned char *msg) const {
  CryptoPP::Integer m{msg, msg_size_};
  auto ci = private_key_.CalculateInverse(prng, m);
  ci.Encode(msg, msg_size_);
}

void Rsa::generate_private_and_public_key(std::stringstream &ss) const {
  CryptoPP::AutoSeededRandomPool rng;
  CryptoPP::InvertibleRSAFunction params;

  params.GenerateRandomWithKeySize(rng, key_size_);

  auto gen_public_key = CryptoPP::RSA::PublicKey(params);
  auto gen_private_key = CryptoPP::RSA::PrivateKey(params);

  CryptoPP::Integer const &n = params.GetModulus();
  CryptoPP::Integer const &p = params.GetPrime1();
  CryptoPP::Integer const &q = params.GetPrime2();
  CryptoPP::Integer const &d = params.GetPrivateExponent();
  CryptoPP::Integer const &e = params.GetPublicExponent();

  auto to_string = [](const CryptoPP::RSAFunction& key) {
    std::string string_key;
    CryptoPP::Base64Encoder key_sink;

    key.DEREncode(key_sink);
    key_sink.MessageEnd();

    auto size = key_sink.MaxRetrievable();
    if (size) {
      string_key.resize(size);
      key_sink.Get((unsigned char *)&string_key[0], string_key.size());
    }

    return string_key;
  };

  ss  << "Public key values:" << std::endl
                    << " (HEX) n: " << std::hex << n << std::endl
                    << " n: " << std::dec << n << std::endl
                    << " (HEX) e: " << std::hex << e << std::endl
                    << " e: " << std::dec << e << std::endl
                    << " public PEM: " << std::endl
                    << "-----BEGIN PUBLIC KEY-----" << std::endl
     << to_string(gen_public_key) << "-----END PUBLIC KEY-----"
                    << std::endl;

  ss << "Private key values:" << std::endl
     << " (HEX) p: " << std::hex << p << std::endl
     << " p: " << std::dec << p << std::endl
     << " (HEX) q: " << std::hex << q << std::endl
     << " q: " << std::dec << q << std::endl
     << " (HEX) d: " << std::hex << d << std::endl
     << " d: " << std::dec << d << std::endl
     << " private PEM: " << std::endl
     << "-----BEGIN RSA PRIVATE KEY-----" << std::endl
     << to_string(gen_private_key) << "-----END RSA PRIVATE KEY-----"
     << std::endl;
}

void Rsa::set_public_key(const char *n, const char *e) {
  CryptoPP::Integer iN{n};
  CryptoPP::Integer iE{e};
  public_key_.Initialize(iN, iE);
}

void Rsa::set_private_key(const char *n, const char *e, const char *d) {
  CryptoPP::Integer iN{n};
  CryptoPP::Integer iE{e};
  CryptoPP::Integer iD{d};

  private_key_.Initialize(iN, iE, iD);
}

