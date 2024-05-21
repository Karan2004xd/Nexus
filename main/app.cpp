#include "server/include/Server.hpp"
#include "constants.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string getFileContent(const std::string &fileName) {
  std::ostringstream oss;
  oss << MAIN_DIR << "/" << fileName;
  std::fstream file {oss.str()};

  if (file.is_open()) {
    oss.clear();
    oss.str(std::string{});
    oss << file.rdbuf();
  } else {
    throw std::runtime_error("File not opened");
  }
  file.close();
  return oss.str();
}

int main() {
  Nexus::Server::run();
  /* std::string fileName = "nexus.txt"; */
  /* std::string fileContent = getFileContent(fileName); */
  /* Dfs dfs; */
  /* auto result = dfs.deteteTrashDataApi(fileName); */
  /* if (result.isSuccess()) { */
  /*   std::cout << result.getOutput() << std::endl; */
  /* } else { */
  /*   std::cout << result.getErrorMsg() << std::endl; */
  /* } */
  return 0;
}
