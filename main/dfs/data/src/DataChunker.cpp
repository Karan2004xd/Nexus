#include "../include/DataChunker.hpp"
#include "../include/Chunk.hpp"

void DataChunker::setDefaultChunkRatio(int MainDataSize) {
  this->MainDataSize = MainDataSize;

  if (MainDataSize > 1000) {
    defaultChunkRatio = 0.1;
  } else if (MainDataSize > 100) {
    defaultChunkRatio = 0.2;
  } else if (MainDataSize > 10) {
    defaultChunkRatio = 0.25;
  } else if (MainDataSize > 5) {
    defaultChunkRatio = 0.5;
  } else {
    defaultChunkRatio = 1;
  }
}
void DataChunker::setChunks() {
  int chunkNumber {1};
  // Implement Chunking with encryption
}

std::vector<Chunk> DataChunker::getChunks() {
  return this->chunks;
}

void DataChunker::chunkData(const std::string &mainData, int mainDataLength) {
  setDefaultChunkRatio(mainDataLength);
}

