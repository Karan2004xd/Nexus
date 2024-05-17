#ifndef CACHE_HPP
#define CACHE_HPP
#include "../../dfs/include/Nexus.hpp"
#include "Chunk.hpp"
#include <memory>
#include <string>
#include <unordered_map>

using namespace Nexus;

class Data::Cache : protected Chunk { 
protected:
  typedef std::unordered_map<size_t, std::unique_ptr<Chunk>> ChunkedData;
  virtual void storeData(const ChunkedData &chunks);
  virtual void getData(const std::string &fileName);
  virtual void deleteData();
  virtual ~Cache();
private:
  bool checkDirectory();
};

#endif // CACHE_HPP
