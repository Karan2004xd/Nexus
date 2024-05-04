#include <cryptopp/sha.h>
#include <iostream>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <string>

std::string generateKey(size_t keySize) {
  CryptoPP::AutoSeededRandomPool rng;
  CryptoPP::SecByteBlock key(keySize);
  rng.GenerateBlock(key, keySize);

  std::string keyStr;
  CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(keyStr));
  encoder.Put(key, keySize);
  encoder.MessageEnd();
  return keyStr;
}

std::string encryptString(const std::string &plainText, const std::string &key) {
  std::string encryptedText;

  CryptoPP::AES::Encryption aesEncryption(reinterpret_cast<const unsigned char *>(key.data()), CryptoPP::AES::DEFAULT_KEYLENGTH);
  CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, reinterpret_cast<const unsigned char *>(key.data()));

  CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(encryptedText));
  stfEncryptor.Put(reinterpret_cast<const unsigned char *>(plainText.data()), plainText.size());
  stfEncryptor.MessageEnd();

  return encryptedText;
}

std::string decryptString(const std::string &encryptedText, const std::string &key) {
  std::string decryptedText;

  CryptoPP::AES::Decryption aesDecryption(reinterpret_cast<const unsigned char *>(key.data()), CryptoPP::AES::DEFAULT_KEYLENGTH);
  CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryptor(aesDecryption, reinterpret_cast<const unsigned char *>(key.data()));

  CryptoPP::StringSource ss(encryptedText, true, new CryptoPP::StreamTransformationFilter(cbcDecryptor, new CryptoPP::StringSink(decryptedText)));
  return decryptedText;
}

int bucketNum(const std::string &chunk, int numBuckets) {
  CryptoPP::SHA256 hash;
  CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
  hash.CalculateDigest(digest, (CryptoPP::byte *) chunk.c_str(), chunk.length());

  unsigned int hashValue = *((unsigned int *) digest);
  return hashValue % (numBuckets - 1);
}

int main() {
  size_t keySize = 16;
  std::string randomKey = generateKey(keySize);
  std::string encryptedString = encryptString("Hello World", randomKey);
  std::string decryptedString = decryptString(encryptedString, randomKey);
  std::cout << "Random Key: " << randomKey << "\n\rEncrypted Text: " << encryptedString << std::endl;
  std::cout << "Decrypted Text: " << decryptedString << std::endl;

  std::cout << bucketNum(encryptedString, 5) << std::endl;
  return 0;
}

