#ifndef DATA_CHUNKER_HPP
#define DATA_CHUNKER_HPP
#include "Chunk.hpp"

#include <memory>
#include <vector>

class DataChunker {
protected:
  void chunkData(const std::string &, int, const std::string &);

private:
  std::string MainData;
  int MainDataSize;
  std::string contentType;
  std::vector<std::unique_ptr<Chunk>> chunks;
  
  double defaultChunkRatio;

  size_t findChunkSize(const std::string &);
  void setDefaultChunkRatio(int);
  void setChunks();

  std::vector<std::unique_ptr<Chunk>> getChunks();
  void setMainData(const std::string &, const std::string &);
};
#endif // DATA_CHUNKER_HPP
