#include "../include/Chunk.hpp"
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <iostream>

Chunk::Chunk(const std::string &chunkContent, size_t chunkSize)
  : chunkContent(chunkContent), chunkSize(chunkSize) {
  generateChunkId();
  encryptChunkData();
}

void Chunk::generateChunkId() {
  CryptoPP::AutoSeededRandomPool rng;
  CryptoPP::SecByteBlock id {DEFAULT_CHUNK_ID_SIZE};
  rng.GenerateBlock(id, DEFAULT_CHUNK_ID_SIZE);

  std::string tempChunkId;
  CryptoPP::HexEncoder encoder {new CryptoPP::StringSink{tempChunkId}};
  encoder.Put(id, DEFAULT_CHUNK_ID_SIZE);
  encoder.MessageEnd();

  std::cout << tempChunkId << std::endl;
  this->chunkId = tempChunkId;
}

void Chunk::encryptChunkData() {
  std::string tempEncryptedData;

  CryptoPP::AES::Encryption aesEncryption {reinterpret_cast<const unsigned char *>(chunkId.data()),
                                           CryptoPP::AES::DEFAULT_KEYLENGTH};
  CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption {aesEncryption,
                                                               reinterpret_cast<const unsigned char *>(chunkId.data())};

  CryptoPP::StreamTransformationFilter stfEncryptor {cbcEncryption,
                                                     new CryptoPP::StringSink {tempEncryptedData}};
  stfEncryptor.Put(reinterpret_cast<const unsigned char *>(this->chunkContent.data()),
                   this->chunkContent.size());
  stfEncryptor.MessageEnd();
  this->chunkContent = tempEncryptedData;
}

std::string Chunk::decryptChunkData() {
  std::string tempDecryptedData;
  CryptoPP::AES::Decryption aesDecryption {reinterpret_cast<const unsigned char *>(this->chunkId.data()),
                                           CryptoPP::AES::DEFAULT_KEYLENGTH};
  CryptoPP::CBC_CTS_Mode_ExternalCipher::Decryption cbcDecryptor {aesDecryption,
                                                      reinterpret_cast<const unsigned char *>(this->chunkId.data())};
  CryptoPP::StringSource ss {this->chunkContent,
                             true,
                             new CryptoPP::StreamTransformationFilter {cbcDecryptor,
                                                                       new CryptoPP::StringSink{tempDecryptedData}}};
  return tempDecryptedData;
}
