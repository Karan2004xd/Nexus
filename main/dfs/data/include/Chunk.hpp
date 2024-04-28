#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <string>

class Chunk {
public:
  Chunk() = delete; 
  Chunk(const std::string &, size_t chunkSize);

  const size_t getChunkSize() { return this->chunkSize; }

  const std::string getChunkContent() { return this->chunkContent; }
  void decryptChunkData(const std::string &);

private:
  size_t chunkSize;
  std::string chunkContent;

  const size_t DEFAULT_CHUNK_ID_SIZE = 16;

  std::string generateChunkId();
  void encryptChunkData(const std::string &);
};
#endif // CHUNK_HPP
