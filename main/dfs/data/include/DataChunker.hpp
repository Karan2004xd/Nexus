#ifndef DATA_CHUNKER_HPP
#define DATA_CHUNKER_HPP

#include <string>
#include <vector>

class Chunk;

class DataChunker {
protected:
  void chunkData(const std::string &, int);
private:
  std::string MainData;
  int MainDataSize;
  std::vector<Chunk> chunks;
  
  double defaultChunkRatio;
  void setDefaultChunkRatio(int);
  void setChunks();
  std::vector<Chunk> getChunks();
};
#endif // DATA_CHUNKER_HPP
