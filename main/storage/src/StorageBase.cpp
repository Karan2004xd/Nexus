#include "../include/StorageBase.hpp"

void StorageBase::setChunks(const std::vector<std::unique_ptr<Chunk>> &original) {
  for (const auto &chunk : original) {
    this->chunks.push_back(std::make_unique<Chunk>(*chunk->clone()));
  }
}
