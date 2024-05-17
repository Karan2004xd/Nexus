#ifndef STORAGE_BASE_HPP
#define STORAGE_BASE_HPP
#include <string>

class StorageBase {
protected:
  virtual void storeData(const std::string &data) = 0;
  virtual std::string getData(const std::string &identifier) = 0;
  virtual void deleteData(const std::string &identifier) = 0;
};
#endif // STORAGE_BASE_HPP
