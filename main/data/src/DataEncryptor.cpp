#include "../include/DataEncryptor.hpp"

#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>

std::string DataEncryptor::generateDataKey() {
  CryptoPP::AutoSeededRandomPool range;
  CryptoPP::SecByteBlock key {DEFAULT_CHUNK_KEY_SIZE};
  range.GenerateBlock(key, DEFAULT_CHUNK_KEY_SIZE);

  std::string tempKeyValue;
  CryptoPP::HexEncoder encoder {new CryptoPP::StringSink{tempKeyValue}};
  encoder.Put(key, DEFAULT_CHUNK_KEY_SIZE);
  encoder.MessageEnd();

  return tempKeyValue;
}

std::pair<std::string, std::string> DataEncryptor::encryptData(const std::string &data) {
  std::string encryptedData;
  std::string key = generateDataKey();

  CryptoPP::AES::Encryption aesEncryption {reinterpret_cast<const unsigned char *>(key.data()),
                                           CryptoPP::AES::DEFAULT_KEYLENGTH};
  CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption {aesEncryption,
                                                               reinterpret_cast<const unsigned char *>(key.data())};

  CryptoPP::StreamTransformationFilter stfEncryptor {cbcEncryption,
                                                     new CryptoPP::StringSink {encryptedData}};
  stfEncryptor.Put(reinterpret_cast<const unsigned char *>(data.data()),
                   data.size());
  stfEncryptor.MessageEnd();
  return std::make_pair(key, encryptedData);
}

std::string DataEncryptor::decryptData(const std::string &key, const std::string &data) {
  std::string decryptedData;
  CryptoPP::AES::Decryption aesDecryption (reinterpret_cast<const unsigned char *>(key.data()),
                                           CryptoPP::AES::DEFAULT_KEYLENGTH);
  CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryptor (aesDecryption,
                                                              reinterpret_cast<const unsigned char *>(key.data()));
  CryptoPP::StringSource ss (data,
                             true,
                             new CryptoPP::StreamTransformationFilter (cbcDecryptor,
                                                                       new CryptoPP::StringSink{decryptedData}));
  return decryptedData;
}
