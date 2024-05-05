#include "../include/Dfs.hpp"


Dfs::Dfs() {
  this->dataHandler = std::make_unique<Data::Handler>();
}

std::string renameFile();
std::string deleteFile();
std::string getFile();
