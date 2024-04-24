#ifndef DATA_CHUNKER_HPP
#define DATA_CHUNKER_HPP

#include "Data.hpp"

#include <memory>
#include <vector>

#include "Chunk.hpp"

class Data::DataChunker {
public:
  DataChunker(const Content &);
  const std::vector<std::unique_ptr<Chunk>> &getChunks();

private:
  std::string MainData;
  int MainDataSize;
  std::vector<std::unique_ptr<Chunk>> chunks;

  double defaultChunkRatio;

  size_t findChunkSize(const std::string &);
  void setDefaultChunkRatio(int);
  void setChunks();

  void setMainData(const std::string &, int);
};
#endif // DATA_CHUNKER_HPP
