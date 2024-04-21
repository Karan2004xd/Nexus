#ifndef DATA_CHUNKER_HPP
#define DATA_CHUNKER_HPP

#include <memory>
#include <string>
#include <vector>
#include "Chunk.hpp"

class DataChunker {
protected:
  void chunkData(const std::string &, int);

private:
  std::string MainData;
  int MainDataSize;
  std::vector<std::unique_ptr<Chunk>> chunks;
  
  double defaultChunkRatio;

  size_t findChunkSize(const std::string &);
  void setDefaultChunkRatio(int);
  void setChunks();

  std::vector<std::unique_ptr<Chunk>> getChunks();
  void setMainData(const std::string &);
};
#endif // DATA_CHUNKER_HPP
