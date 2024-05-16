#include "metadata/include/MetaData.hpp"
#include "server/include/Server.hpp"
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
  /*   .singleData("file", "parser_test_query.sql") */
  /*   .singleData("name", "user13") */
  /*   .singleData("id", "13") */
  /*   .getJsonData(); */

  /* auto query = Nexus::Utils::SimpleQueryParser::parseQuery(TEST_QUERIES_DIR, jsonData); */
  /* std::cout << query.getParsedData() << std::endl; */
  /* Nexus::Server::run(); */

  /* Nexus::MetaData metadata; */
  /* std::cout << query.getParsedData() << std::endl; */
  /* metadata.updateData(query); */
  /* auto dbData = metadata.getQueryDataMap(query); */
  /* metadata.printData(dbData); */
  return 0;
}
