#ifndef DATABASE_HANDLER_HPP
#define DATABASE_HANDLER_HPP

#include "Database.hpp"
#include "QueryParser.hpp"
#include "../../utility/include/JsonStringBuilder.hpp"

#include <mysql/mysql.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../../variables.h"

class Database::DatabaseHandler : protected QueryParser {
public:
  DatabaseHandler();

  void updateData(const Utility::JsonStringBuilder &);

  std::unordered_map<int, std::vector<std::string>> getDataByRow(const Utility::JsonStringBuilder &);
  std::unordered_map<int, std::vector<std::string>> getDataByColumn(const Utility::JsonStringBuilder &);
  void printData(std::unordered_map<int, std::vector<std::string>> &);

  ~DatabaseHandler();
private:
  std::string query;

  void mysqlConnectionSetup();
  void checkConnection();

  void setQuery(const Utility::JsonStringBuilder &);

  struct ConnectionDetails {
    const char *server = D_SERVER;
    const char *database = DATABASE;
    const char *username = USERNAME;
    const char *password = PASSWORD;
  };

  MYSQL *connection {nullptr};
  MYSQL_RES *executeQuery(const std::string &);
};
#endif // DATABASE_HANDLER_HPP
