#include "../include/DbConnHandler.hpp"
#include <iostream>

void DbConnHandler::setupConnection() {
  connection = mysql_init(NULL);
  struct ConnectionDetails connDetails;
  auto conn = mysql_real_connect(connection,
                                 connDetails.server,
                                 connDetails.username,
                                 connDetails.password,
                                 connDetails.database, 0, NULL, 0);

  if (!conn) {
    throw std::runtime_error("(DbConnHandler) : Connection Error : " + std::string(mysql_error(connection)));
  } else {
    std::cout << "Connected" << std::endl;
  }
}

void DbConnHandler::checkConnection() {
  if (connection == nullptr) {
    setupConnection();
  }
}

void DbConnHandler::closeConnection() {
  if (connection != nullptr) {
    mysql_close(connection);
  }
}
