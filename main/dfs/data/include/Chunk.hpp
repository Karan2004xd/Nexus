#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <string>

class Chunk {
public:
  Chunk(int chunkNumber, size_t chunkSize, const std::string &chunkName)
    : chunkNumber(chunkNumber), chunkSize(chunkSize), chunkName(chunkName) {}

  int getChunkNumber() { return this->chunkNumber; }
  size_t getChunkSize() { return this->chunkSize; }
  std::string getChunkName() { return this->chunkName; }
private:
  int chunkNumber;
  size_t chunkSize;
  std::string chunkName;
};
#endif // CHUNK_HPP
