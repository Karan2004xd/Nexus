#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <string>

class Chunk {
public:
  Chunk() = delete; 
  Chunk(const std::string &, const std::string &, size_t);

  const size_t getChunkSize() { return this->chunkSize; }
  const std::string getChunkContent() { return this->chunkContent; }
  const std::string getChunkId() { return this->chunkId; }

  const std::string getBucketNum() { return this->bucketNum; }
  const std::string getObjectKey() { return this->objectKey; }

  void decryptChunkData(const std::string &);

private:
  size_t chunkSize;
  std::string fileId;
  std::string chunkId;

  std::string chunkContent;
  std::string bucketNum;
  std::string objectKey;

  const size_t DEFAULT_CHUNK_ID_SIZE = 16;
  const size_t NUMBER_OF_BUCKETS = 5;

  std::string generateChunkId();
  void encryptChunkData(const std::string &);
  int bucketToAssign();
};
#endif // CHUNK_HPP
