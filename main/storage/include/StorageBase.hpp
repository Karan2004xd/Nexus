#ifndef STORAGE_BASE_HPP
#define STORAGE_BASE_HPP
#include "../../data/include/Chunk.hpp"

class StorageBase {
protected:
  virtual void storeData(const std::string &data) = 0;
  virtual std::string getData(const std::string &identifier) = 0;
  virtual void deleteData(const std::string &identifier) = 0;

  std::vector<std::unique_ptr<Chunk>> chunks;
  void setChunks(const std::vector<std::unique_ptr<Chunk>> &original);
  virtual ~StorageBase() = default;
};
#endif // STORAGE_BASE_HPP
