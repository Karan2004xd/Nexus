#ifndef DATABASE_HANDLER_HPP
#define DATABASE_HANDLER_HPP
#include "../../../variables.h"
#include <mysql/mysql.h>
#include <iostream>

class DatabaseHandler {
protected:
  static void checkConnection();

private:
  static void mysqlConnectionSetup();

  struct ConnectionDetails {
    const char *server = SERVER;
    const char *database = DATABASE;
    const char *username = USERNAME;
    const char *password = PASSWORD;
  };

  static MYSQL *connection;
};
#endif // DATABASE_HANDLER_HPP
