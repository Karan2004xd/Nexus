#include "dfs/include/Nexus.hpp"
#include "constants.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "server/include/Server.hpp"

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
  Nexus::Server server;
  server.start();
  return 0;
}
