#include "data/include/Chunker.hpp"
#include "data/include/Contents.hpp"
#include "metadata/include/MetaData.hpp"
#include "server/include/Server.hpp"
#include "storage/include/AwsS3.hpp"
#include "utils/include/json/SimpleJsonParser.hpp"
#include "utils/include/query/SimpleQueryParser.hpp"
#include "constants.h"
#include <fstream>
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
  auto jsonData = Nexus::Utils::SimpleJsonParser::JsonBuilder()
    /* .singleData("file", "parser_test_query") */
    /* .singleData("name", "user13") */
    /* .singleData("id", "13") */
    .singleData(FILE_NAME, "nexus.txt")
    .singleData(FILE_CONTENT, getFileContent("nexus.txt"))
    .getJsonData();

  /* auto query = Nexus::Utils::SimpleQueryParser::parseQuery(TEST_QUERIES_DIR, jsonData); */

  /* Nexus::MetaData metadata; */
  /* metadata.updateData(query); */
  /* auto dbData = metadata.getQueryDataMap(query); */
  /* std::cout << dbData["id"][0] << std::endl; */
  /* metadata.printData(dbData); */

  /* Nexus::Server::run(); */
  /* Nexus::Data::Contents contents {jsonData}; */

  /* Nexus::Data::Chunker chunker {contents}; */
  /* auto &chunks = chunker.getChunks(); */

  Storage::AwsS3 aws;
  /* aws.storeData(chunks); */

  /* auto resultChunks = aws.getData(1); */
  /* std::string result; */
  /* for (const auto &chunk : resultChunks) { */
  /*   result += chunk->getDecryptedData(); */
  /* } */

  /* std::cout << "\n"; */
  /* std::cout << result << std::endl; */
  /* std::cout << "\n"; */

  /* aws.deleteData(1); */
  /* auto restoredChunks = aws.getBackupData(1); */
  /* std::string result; */
  /* for (const auto &chunk : restoredChunks) { */
  /*   result += chunk->getDecryptedData(); */
  /* } */
  /* std::cout << "\n"; */
  /* std::cout << result << std::endl; */
  /* std::cout << "\n"; */
  aws.deleteBackupData(1);
  /* aws.restoreData(restoredChunks, 1); */
  return 0;
}
