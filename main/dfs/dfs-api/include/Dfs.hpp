#ifndef DFS_HPP
#define DFS_HPP
#include <memory>
#include <string>

#include "../../data/include/Handler.hpp"

class Dfs {
public:
  Dfs(); 
  static std::string storeFile(const std::string &, const std::string &);
  static std::string renameFile();
  static std::string deleteFile(const std::string &);
  static std::string getFile(const std::string &);
private:
  static std::unique_ptr<Data::Handler> dataHandler;
};
#endif // DFS_HPP
