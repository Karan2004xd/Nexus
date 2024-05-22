#include "data/include/Cache.hpp"
#include "dfs/include/Nexus.hpp"
#include "server/include/Server.hpp"
#include "data/include/Contents.hpp"
#include "data/include/Chunker.hpp"
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
  /* std::string fileName = "2wallpaperflare.com_wallpaper.jpg"; */
  /* std::string fileContent = getFileContent("1wallpaperflare.com_wallpaper.jpg"); */
  /* Nexus::Data::Contents contents {fileName, fileContent}; */
  /* Nexus::Data::Chunker chunker {contents}; */
  /* auto &chunks = chunker.getChunks(); */
  /* Nexus::Data::Cache cache; */
  /* cache.storeData(chunks); */
  Nexus::Server::run();

  /* for (const auto &chunk : chunks) { */
    
  /* } */
  return 0;
}
