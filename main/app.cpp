/* #include "./dfs/server/include/Server.hpp" */
#include "dfs/database/include/Metadata.hpp"
#include <iostream>

int main() {
  /* Server server; */
  /* server.start(); */

  Database::Metadata metaData;
  /* metaData.storeData("INSERT INTO test (id, name) VALUES (4, 'user2')"); */
  auto result = metaData.getDataByRow("SELECT column_name FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name = 'test'");
  metaData.printData(result);
  /* std::cout << result[2][0] << std::endl; */
  return 0;
}
