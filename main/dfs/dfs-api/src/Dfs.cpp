#include "../include/Dfs.hpp"
#include "../../data/include/Content.hpp"
#include "../../data/include/DataChunker.hpp"
#include "../../data/include/Handler.hpp"
#include "../../server/include/Server.hpp"
#include <exception>

Dfs::Dfs() {
  this->dataHandler = std::make_unique<Data::Handler>();
  Server server;
  server.start();
}

std::string Dfs::storeFile(const std::string &rawData,
                           const std::string &fileName) {
  std::string info;
  try {
    Data::Content content {rawData, fileName};
    Data::DataChunker chunker {content};
    dataHandler->storeDataToStorage(chunker);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    info = "The File cannot be created due to some error. Please try again\n";
  }
  return info;
}

std::string renameFile();

std::string Dfs::deleteFile(const std::string &fileName) {
  std::string info;
  try {
    dataHandler->deleteDataFromStorage(fileName);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    info = "The File cannot be deleted, check if the same file doesn't exist\n";
  }
  return info;
}

std::string Dfs::getFile(const std::string &fileName) {
  std::string info;
  try {
    dataHandler->getDataFromStorge(fileName);
  } catch (std::exception &e) {
    info = "Unable to fetch the file data, check if the file exists or not corrupted\n";
    std::cout << e.what() << std::endl;
  }
  return info;
}
