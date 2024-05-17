#include "data/include/Chunker.hpp"
#include "data/include/Contents.hpp"
#include "metadata/include/MetaData.hpp"
#include "server/include/Server.hpp"
#include "storage/include/AwsS3.hpp"
#include "utils/include/json/SimpleJsonParser.hpp"
#include "utils/include/query/SimpleQueryParser.hpp"
#include "constants.h"
#include <functional>
#include <iostream>

const uint64_t spaceSize = 1 << 30;

int getHashValue(const std::string &data, int numBuckets) {
  std::hash<std::string> strHash;
  uint64_t hash = strHash(data);
  return (hash % spaceSize) % numBuckets;
}

int main() {
  /* auto jsonData = Nexus::Utils::SimpleJsonParser::JsonBuilder() */
  /*   .singleData("file", "parser_test_query") */
  /*   .singleData("name", "user13") */
  /*   .singleData("id", "13") */
  /*   .getJsonData(); */

  /* auto query = Nexus::Utils::SimpleQueryParser::parseQuery(TEST_QUERIES_DIR, jsonData); */
  /* std::cout << query.getParsedData() << std::endl; */

  /* Nexus::MetaData metadata; */
  /* metadata.updateData(query); */
  /* auto dbData = metadata.getQueryDataMap(query); */
  /* std::cout << dbData["id"][dbData["id"].size() - 1] << std::endl; */
  /* metadata.printData(dbData); */

  /* Nexus::Server::run(); */
  /* Nexus::Data::Contents contents {jsonData}; */

  Nexus::Data::Chunker chunker {};
  auto &chunks = chunker.getChunks();
  /* std::string fileData; */
  /* for (const auto &chunk : *chunks) { */
  /*   fileData += chunk->getDecryptedData(); */
  /* } */

  Storage::AwsS3 aws {chunks};
  return 0;
}
