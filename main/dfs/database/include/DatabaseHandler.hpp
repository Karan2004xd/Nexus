#ifndef DATABASE_HANDLER_HPP
#define DATABASE_HANDLER_HPP

#include "../../../variables.h"
#include <mysql/mysql.h>
#include <string>
#include <unordered_map>
#include <vector>

class DatabaseHandler {
public:
  void checkConnection();

  void storeData(const std::string &);

  std::unordered_map<int, std::vector<std::string>> getDataByRow(const std::string &);
  std::unordered_map<int, std::vector<std::string>> getDataByColumn(const std::string &);
  void printData(std::unordered_map<int, std::vector<std::string>> &);

  ~DatabaseHandler();
private:
  void mysqlConnectionSetup();

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
