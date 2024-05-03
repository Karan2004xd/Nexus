#include "../include/Chunk.hpp"
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>

Chunk::Chunk(const std::string &chunkContent, size_t chunkSize)
  : chunkContent(chunkContent), chunkSize(chunkSize) {
  std::string chunkId = generateChunkId();

  encryptChunkData(chunkId);
  // Implement a database api for storing the chunkId
}

std::string Chunk::generateChunkId() {
  CryptoPP::AutoSeededRandomPool rng;
  CryptoPP::SecByteBlock id {DEFAULT_CHUNK_ID_SIZE};
  rng.GenerateBlock(id, DEFAULT_CHUNK_ID_SIZE);

  std::string tempChunkId;
  CryptoPP::HexEncoder encoder {new CryptoPP::StringSink{tempChunkId}};
  encoder.Put(id, DEFAULT_CHUNK_ID_SIZE);
  encoder.MessageEnd();

  return tempChunkId;
}

void Chunk::encryptChunkData(const std::string &chunkId) {
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

void Chunk::decryptChunkData(const std::string &chunkId) {
  std::string tempDecryptedData;
  CryptoPP::AES::Decryption aesDecryption {reinterpret_cast<const unsigned char *>(chunkId.data()),
                                           CryptoPP::AES::DEFAULT_KEYLENGTH};
  CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryptor {aesDecryption,
                                                      reinterpret_cast<const unsigned char *>(chunkId.data())};
  CryptoPP::StringSource ss {this->chunkContent,
                             true,
                             new CryptoPP::StreamTransformationFilter {cbcDecryptor,
                                                                       new CryptoPP::StringSink{tempDecryptedData}}};
  this->chunkContent = tempDecryptedData;
}
