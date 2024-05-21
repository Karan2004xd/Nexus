#ifndef STORAGE_BASE_HPP
#define STORAGE_BASE_HPP
#include "../../data/include/Chunk.hpp"

class StorageBase {
protected:
  virtual void storeData(const std::vector<std::unique_ptr<Chunk>> &chunks) = 0;

  virtual std::vector<std::unique_ptr<Chunk>> getData(const size_t &fileId) = 0;
  virtual std::vector<std::unique_ptr<Chunk>> getBackupData(const size_t &fileId) = 0;

  virtual void deleteData(const size_t &fileId) = 0;
  virtual void deleteBackupData(const size_t &fileId) = 0;

  /* virtual void restoreData(const size_t &filedId) = 0; */

  virtual ~StorageBase() = default;
};
#endif // STORAGE_BASE_HPP
