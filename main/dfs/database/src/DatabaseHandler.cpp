#include "../include/DatabaseHandler.hpp"
#include <iostream>

void DatabaseHandler::checkConnection() {
  if (connection == nullptr) {
    mysqlConnectionSetup();
  }
}

void DatabaseHandler::mysqlConnectionSetup() { 
  connection = mysql_init(NULL);
  struct ConnectionDetails conn_details;
  if (!mysql_real_connect(connection,
                          conn_details.server,
                          conn_details.username,
                          conn_details.password,
                          conn_details.database, 0, NULL, 0)) {
    std::cout << "Connection error: " << mysql_error(connection) << std::endl;
    exit(1);
  } else {
    std::cout << "Connected to database" << std::endl;
  }
}

MYSQL_RES *DatabaseHandler::executeQuery(const char *sqlQuery) {
  int runQuery = mysql_query(connection, sqlQuery);
  MYSQL_RES *result {nullptr};
  if (runQuery) {
    std::cout << "Mysql Query Error: " << mysql_error(connection) << std::endl;
  } else {
    result = mysql_use_result(connection);
  }
  return result;
}

std::unordered_map<int, std::vector<std::string>> DatabaseHandler::getDataByRow(const std::string &query) {
  MYSQL_ROW row;
  MYSQL_RES *result = executeQuery(query.c_str());
  // start working from here
}

std::unordered_map<int, std::vector<std::string>> DatabaseHandler::getDataByColumn(const std::string &query) {

}

DatabaseHandler::~DatabaseHandler() {
  mysql_close(connection);
}
