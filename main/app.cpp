#include "data/include/Chunker.hpp"
#include "data/include/Contents.hpp"
#include "metadata/include/MetaData.hpp"
#include "server/include/Server.hpp"
#include "storage/include/AwsS3.hpp"
#include "utils/include/json/SimpleJsonParser.hpp"
#include "utils/include/query/SimpleQueryParser.hpp"
#include "constants.h"
#include <chrono>
#include <fstream>
#include <future>
#include <iostream>
#include <thread>

std::string getFileContent(const std::string &fileName) {
  std::ostringstream oss;
  oss << MAIN_DIR << "/" << fileName;
  std::fstream file {oss.str()};

  if (file.is_open()) {
    oss.clear();
    oss << file.rdbuf();
  } else {
    throw std::runtime_error("File not opened");
  }
  file.close();
  return oss.str();
}

/* void backgroundTask(int duration) { */
/*   std::cout << "Background Task started" << std::endl; */
/*   std::this_thread::sleep_for(std::chrono::seconds(duration)); */
/*   std::cout << "Background Task finished" << std::endl; */
/* } */

int main() {
  auto jsonData = Nexus::Utils::SimpleJsonParser::JsonBuilder()
    /* .singleData("file", "parser_test_query") */
    /* .singleData("name", "user13") */
    /* .singleData("id", "13") */
    .singleData(FILE_NAME, "nexus.txt")
    .singleData(FILE_CONTENT, getFileContent("nexus.txt"))
    .getJsonData();

  /* auto query = Nexus::Utils::SimpleQueryParser::parseQuery(TEST_QUERIES_DIR, jsonData); */
  /* std::cout << query.getParsedData() << std::endl; */

  /* Nexus::MetaData metadata; */
  /* metadata.updateData(query); */
  /* auto dbData = metadata.getQueryDataMap(query); */
  /* std::cout << dbData["id"][dbData["id"].size() - 1] << std::endl; */
  /* metadata.printData(dbData); */

  /* Nexus::Server::run(); */
  Nexus::Data::Contents contents {jsonData};

  Nexus::Data::Chunker chunker {contents};
  auto &chunks = chunker.getChunks();

  Storage::AwsS3 aws {chunks};

  /* std::future<void> bgFuture = std::async(std::launch::async, backgroundTask, 5); */
  /* std::cout << "Main thread free to work" << std::endl; */
  /* std::this_thread::sleep_for(std::chrono::seconds(7)); */
  /* std::cout << "Main thread work done" << std::endl; */
  return 0;
}
