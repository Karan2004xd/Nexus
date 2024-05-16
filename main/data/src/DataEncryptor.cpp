#include "../include/DataEncryptor.hpp"

#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/sha.h>

std::string DataEncryptor::generateDataKey() {
  CryptoPP::AutoSeededRandomPool range;
  CryptoPP::SecByteBlock key {DEFAULT_CHUNK_KEY_SIZE};
  range.GenerateBlock(key, DEFAULT_CHUNK_KEY_SIZE);
  // Start from here
}
