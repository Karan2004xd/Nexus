#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "../../metadata/include/MetaData.hpp"
#include "DataEncryptor.hpp"
#include <string>
#include <memory>

class Chunk : protected DataEncryptor {
public:
  Chunk() = delete;
  Chunk(const std::string &rawchunk, const size_t &fileId);
  Chunk(const std::string &encryptedData, const std::string &chunkKey);

  Chunk(const std::string &objectKey,
        const std::string &chunkKey,
        const std::string &data,
        const size_t &fileId);

  Chunk(const Chunk &other);

  const std::string getDecryptedData();
  const std::string getEncryptedData();
  const std::string getObjectKey() { return this->objectKey; }

  std::unique_ptr<Chunk> clone();

private:
  const int DEFAULT_OBJECT_KEY_SIZE = 10;

  Nexus::MetaData metaData;

  // (key, data)
  std::pair<std::string, std::string> data;
  std::string objectKey;

  size_t chunkId;
  size_t fileId;

  void setFileId(const size_t &fileId) { this->fileId = fileId; }
  void setRawData(const std::string &fileName);

  void setChunkId();
  void setObjectKey();
  void updateMetadata();

  void setData(const std::string &encryptedData,
               const std::string &chunkKey);

  void setupChunk(const std::string &rawChunkData,
                  const size_t &fileId);
};
#endif // CHUNK_HPP
