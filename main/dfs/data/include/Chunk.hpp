#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <string>

class Chunk {
public:
  Chunk() = delete; 
  Chunk(const std::string &, size_t chunkSize);

  const size_t getChunkSize() { return this->chunkSize; }
  const std::string getChunkName() { return this->chunkName; }

  const std::string getChunkContent() { return this->chunkContent; }
  const std::string getChunkId() { return this->chunkId; }
  std::string decryptChunkData();

private:
  size_t chunkSize;
  std::string chunkName;

  std::string chunkId;
  std::string chunkContent;

  const size_t DEFAULT_CHUNK_ID_SIZE = 16;

  void generateChunkId();
  void encryptChunkData();
};
#endif // CHUNK_HPP
