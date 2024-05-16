#ifndef DATA_ENCRYPTOR_HPP
#define DATA_ENCRYPTOR_HPP
#include <string>

class DataEncryptor {
protected:
  std::pair<std::string, std::string> encryptData(const std::string &data);

private:
  const size_t DEFAULT_CHUNK_KEY_SIZE = 16;
  std::string generateDataKey();
};
#endif // DATA_ENCRYPTOR_HPP
