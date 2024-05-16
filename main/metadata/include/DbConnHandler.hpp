#ifndef DB_CONN_HANDLER_HPP
#define DB_CONN_HANDLER_HPP
#include "../../variables.h"
#include <mysql/mysql.h>

class DbConnHandler {
protected:
  void checkConnection();
  void closeConnection();

  MYSQL *connection {nullptr};
private:
  struct ConnectionDetails {
    const char *server = D_SERVER;
    const char *database = DATABASE;
    const char *username = USERNAME;
    const char *password = PASSWORD;
  };

  void setupConnection();
};
#endif // DB_CONN_HANDLER_HPP
