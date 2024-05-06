#ifndef DFS_HPP
#define DFS_HPP
#include <memory>
#include <string>

#include "../../data/include/Handler.hpp"

class Dfs {
public:
  Dfs(); 
  std::string storeFile(const std::string &, const std::string &);
  std::string renameFile();
  std::string deleteFile(const std::string &);
  std::string getFile(const std::string &);
private:
  std::unique_ptr<Data::Handler> dataHandler;
};
#endif // DFS_HPP
