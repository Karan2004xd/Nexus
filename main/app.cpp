#include "server/include/Server.hpp"
#include "utils/include/json/SimpleJsonParser.hpp"
#include "utils/include/query/SimpleQueryParser.hpp"
#include <iostream>
#include "constants.h"

int main() {
  auto jsonData = Nexus::Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "parser_test_query.sql")
    .singleData("age", "18")
    .singleData("name_value", "karan")
    .getJsonData();

  /* std::cout << Nexus::Utils::SimpleJsonParser::encodeJson(jsonData) << std::endl; */
  auto query = Nexus::Utils::SimpleQueryParser::parseQuery(TEST_QUERIES_DIR, jsonData).getParsedData();
  std::cout << query << std::endl;
  /* Nexus::Server::run(); */
  return 0;
}
