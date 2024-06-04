#include "dfs/include/Nexus.hpp"
#include "server/include/Server.hpp"
#include "data-transfer/include/DataTransfer.hpp"
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
  /* Nexus::Server::run(); */
  DataTransfer api;
  api.startKafkaApi();
  return 0;
}
