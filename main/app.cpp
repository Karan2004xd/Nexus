/* #include "./dfs/server/include/Server.hpp" */
#include "constants.h"
#include "dfs/database/include/QueryParser.hpp"
#include "dfs/database/include/DatabaseHandler.hpp"
#include "dfs/database/include/JsonStringBuilder.hpp"
#include <iostream>

int main() {
  /* Server server; */
  /* server.start(); */

  /* Database::Metadata metaData; */
  /* auto result = metaData.getDataByRow("SELECT column_name FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name = 'test'"); */
  /* metaData.printData(result); */
  /* std::cout << result[2][0] << std::endl; */

  QueryParser qp;
  /* Database::DatabaseHandler db; */
  /* auto result = db.getDataByRow("SELECT * FROM test"); */
  /* db.printData(result); */
  /* db.storeData("INSERT INTO test (name, id) VALUES ('user3', 5)"); */

  Database::JsonStringBuilder builder;
  builder.singleData("file", std::string(QUERIES_DIR) + "/test/example_query.sql")
         .build();
  auto result = builder.str();
  std::cout << result << std::endl;

  qp.parseJsonData(result);

  /* JsonDataHandler json; */
  /* auto encodedJson = json.encodeStringToJson(result); */
  /* std::cout << encodedJson << std::endl; */

  /* auto decodedJson = json.decodeJsonToString(encodedJson); */
  /* for (const auto &key : decodedJson) { */
  /*   std::cout << key.first << " : "; */
  /*   if (std::holds_alternative<std::string>(key.second)) { */
  /*     std::cout << std::get<std::string>(key.second); */
  /*   } else if (std::holds_alternative<std::vector<std::string>>(key.second)) { */
  /*     auto vec = std::get<std::vector<std::string>>(key.second); */
  /*     for (const auto &vecValue : vec) { */
  /*       std::cout << vecValue << " "; */
  /*     } */
  /*   } */
  /*   std::cout << "\n"; */
  /* } */
  return 0;
}
