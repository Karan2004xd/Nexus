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
  const std::string getDecryptedData();
private:
  std::unique_ptr<Nexus::MetaData> metaData;
  std::pair<std::string, std::string> data;
  std::string objectKey;

  size_t chunkId;
  size_t fileId;

  void setFileId(const size_t &fileId) { this->fileId = fileId; }
  void setRawData(const std::string &fileName);

  void setChunkId();
  void setObjectKey();
  void updateMetadata();

  void setupChunk(const std::string &rawChunkData,
                  const size_t &fileId);
};
#endif // CHUNK_HPP
