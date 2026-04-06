/*
 * utils.cpp
 *
 *  Created on 04/04/2026
 *    Peter "Magnus" Balling
 */

#include "utils.hpp"

#include <cstdint>
#include <memory>
#include <string>

#include <openssl/bio.h>
#include <openssl/evp.h>

#include <crypto++/base32.h>

std::string Base32Encode(const std::string& binary)
{
  CryptoPP::Base32Encoder b32e;
  for (auto c : binary)
    b32e.Put(c);
  b32e.MessageEnd();
  
  std::string encoded;
  auto size = b32e.MaxRetrievable();
  if(size)
  {
    encoded.resize(size);
    b32e.Get((uint8_t*)&encoded[0], encoded.size());
  }

  return encoded;
}

class BIOFreeAll
{
  public:
    void operator()(BIO* p)
    {
      BIO_free_all(p);
    }
};

// Source: https://stackoverflow.com/a/47981510
std::string Base64Encode(const std::string& binary)
{
    std::unique_ptr<BIO, BIOFreeAll> b64(BIO_new(BIO_f_base64()));
    BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL);
    BIO* sink = BIO_new(BIO_s_mem());
    BIO_push(b64.get(), sink);
    BIO_write(b64.get(), binary.data(), binary.size());
    BIO_flush(b64.get());
    const char* encoded;
    const long len = BIO_get_mem_data(sink, &encoded);
    return std::string(encoded, len);
}

// Assumes no newlines or extra characters in encoded string
std::vector<uint8_t> Base64Decode(const std::string& encoded)
{
    std::unique_ptr<BIO, BIOFreeAll> b64(BIO_new(BIO_f_base64()));
    BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL);
    BIO* source = BIO_new_mem_buf(encoded.c_str(), -1); // read-only source
    BIO_push(b64.get(), source);
    const int maxlen = encoded.length() / 4 * 3 + 1;
    std::vector<uint8_t> decoded(maxlen);
    const int len = BIO_read(b64.get(), decoded.data(), maxlen);
    decoded.resize(len);
    return decoded;
}
