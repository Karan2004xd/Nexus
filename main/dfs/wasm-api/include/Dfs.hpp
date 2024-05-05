#ifndef DFS_HPP
#define DFS_HPP
#include <memory>
#include <string>

#include "../../data/include/Handler.hpp"

class Dfs {
public:
  Dfs(); 
  std::string storeFile();
  std::string renameFile();
  std::string deleteFile();
  std::string getFile();
private:
  std::unique_ptr<Data::Handler> dataHandler;
};
#endif // DFS_HPP
