#include "../include/StorageBase.hpp"

std::vector<std::unique_ptr<Chunk>> StorageBase::copyChunks(
  const std::vector<std::unique_ptr<Chunk>> &original) {

  std::vector<std::unique_ptr<Chunk>> copied;
  copied.reserve(original.size());

  for (const auto &chunk : original) {
    copied.push_back(std::make_unique<Chunk>(*chunk));
  }
  return copied;
}
