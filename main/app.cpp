#include "./dfs/server/include/Server.hpp"
#include "./dfs/database/include/DatabaseHandler.hpp"
#include "./dfs/utility/include/JsonStringBuilder.hpp"
#include <iostream>

int main() {
  Server server;
  server.start();
  /* Database::DatabaseHandler db; */
  /* Utility::JsonStringBuilder builder; */
  /* builder.singleData("file", "test/example_two_query").build(); */

  /* auto result = db.getDataByRow(builder); */
  /* std::cout << result.size() << std::endl; */
  /* db.printData(result); */
  return 0;
}
