#include "./dfs/server/include/Server.hpp"
/* #include "dfs/database/include/DatabaseHandler.hpp" */
/* #include "dfs/storage/include/AwsHandler.hpp" */
/* #include "dfs/utility/include/JsonStringBuilder.hpp" */

int main() {
  Server server;
  server.start();

  /* Database::DatabaseHandler db; */
  /* Utility::JsonStringBuilder builder; */

  /* builder.singleData("file", std::string(QUERIES_DIR) + "/test/example_query.sql") */
  /*        .singleData("name", "user12") */
  /*        .singleData("id", "12") */
  /*        .build(); */

  /* db.updateData(builder); */
  /* builder.clear(); */

  /* builder.singleData("file", std::string(QUERIES_DIR) + "/test/example_two_query.sql").build(); */

  /* auto resultTwo = db.getDataByRow(builder); */
  /* db.printData(resultTwo); */
  /* Storage::AwsHandler handler; */
  return 0;
}
