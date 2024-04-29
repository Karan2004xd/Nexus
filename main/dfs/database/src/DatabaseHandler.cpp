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

DatabaseHandler::~DatabaseHandler() {
  mysql_close(connection);
}
