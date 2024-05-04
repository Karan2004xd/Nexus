#include "../include/Chunk.hpp"
#include "../../database/include/DatabaseHandler.hpp"
#include "../../utility/include/JsonStringBuilder.hpp"

#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/sha.h>

Chunk::Chunk(const std::string &fileId, const std::string &chunkContent, size_t chunkSize)
  : chunkContent(chunkContent), chunkSize(chunkSize), fileId(fileId) {
  
  Database::DatabaseHandler db;

  chunkId = generateChunkId();
  encryptChunkData(chunkId);
  
  Utility::JsonStringBuilder builder;
  builder.singleData("file", "data/get_single_file_data")
         .singleData("id", "name")
         .singleData("name", "id")
         .singleData("value", fileId)
         .build();

  auto data = db.getDataByRow(builder);
  objectKey = data[1][0] + '_' + chunkId;
  bucketNum = std::to_string(bucketToAssign());

  builder.clear();

  builder.singleData("file", "data/store_chunk")
         .singleData("chunk_key", chunkId)
         .singleData("chunk_size", std::to_string(chunkSize))
         .singleData("bucket_num", bucketNum)
         .singleData("object_key", objectKey)
         .singleData("file_id", fileId)
         .build();

  db.updateData(builder);
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

int Chunk::bucketToAssign() {
  CryptoPP::SHA256 hash;
  CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
  hash.CalculateDigest(digest, (CryptoPP::byte *) chunkContent.c_str(), chunkContent.length());

  unsigned int hashValue = *((unsigned int *) digest);
  return hashValue % (NUMBER_OF_BUCKETS - 1);
}
