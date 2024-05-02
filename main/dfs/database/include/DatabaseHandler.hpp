#ifndef DATABASE_HANDLER_HPP
#define DATABASE_HANDLER_HPP

#include "Database.hpp"
#include "QueryParser.hpp"

#include <mysql/mysql.h>
#include <string>
#include <unordered_map>
#include <vector>

class Database::DatabaseHandler : protected QueryParser {
public:
  DatabaseHandler(const JsonStringBuilder &);
  void storeData();

  std::unordered_map<int, std::vector<std::string>> getDataByRow();
  std::unordered_map<int, std::vector<std::string>> getDataByColumn();
  void printData(std::unordered_map<int, std::vector<std::string>> &);

  ~DatabaseHandler();
private:
  std::string query;

  void mysqlConnectionSetup();
  void checkConnection();

  struct ConnectionDetails {
    const char *server = SERVER;
    const char *database = DATABASE;
    const char *username = USERNAME;
    const char *password = PASSWORD;
  };

  MYSQL *connection {nullptr};
  MYSQL_RES *executeQuery(const std::string &);
};
#endif // DATABASE_HANDLER_HPP
