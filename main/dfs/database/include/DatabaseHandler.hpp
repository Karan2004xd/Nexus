#ifndef DATABASE_HANDLER_HPP
#define DATABASE_HANDLER_HPP

#include "Database.hpp"
#include "QueryParser.hpp"
#include "../../utility/include/JsonStringBuilder.hpp"

#include <memory>
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

  void updateData(const std::unique_ptr<Utility::JsonStringBuilder> &);
  std::unordered_map<int, std::vector<std::string>> getDataByRow(const std::unique_ptr<Utility::JsonStringBuilder> &);
  std::unordered_map<int, std::vector<std::string>> getDataByColumn(const std::unique_ptr<Utility::JsonStringBuilder> &);

  void printData(std::unordered_map<int, std::vector<std::string>> &);

  ~DatabaseHandler();
private:
  std::string query;

  void mysqlConnectionSetup();
  void checkConnection();

  void setQuery(const Utility::JsonStringBuilder &);
  void setQuery(std::string &);

  struct ConnectionDetails {
    const char *server = D_SERVER;
    const char *database = DATABASE;
    const char *username = USERNAME;
    const char *password = PASSWORD;
  };

  MYSQL *connection {nullptr};
  MYSQL_RES *executeQuery(const std::string &);

  void updateData(std::string &);
  std::unordered_map<int, std::vector<std::string>> getDataByRow(std::string &);
  std::unordered_map<int, std::vector<std::string>> getDataByColumn(std::string &);
};
#endif // DATABASE_HANDLER_HPP
