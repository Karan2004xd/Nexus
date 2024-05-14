#include "server/include/Server.hpp"
#include "utils/include/json/SimpleJsonParser.hpp"
#include <iostream>

int main() {
  auto jsonData = Nexus::Utils::SimpleJsonParser::JsonBuilder()
    .multipleData("friends", {"John Smith", "James", "Ronald"})
    .singleData("age", "18")
    .singleData("name", "user")
    .getJsonData();

  std::cout << Nexus::Utils::SimpleJsonParser::encodeJson(jsonData) << std::endl;
  /* Nexus::Server::run(); */
  return 0;
}
