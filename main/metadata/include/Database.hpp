#ifndef DATABASE_HPP
#define DATABASE_HPP
#include "../../variables.h"
#include <mysql/mysql.h>

class Database {
protected:
  void checkConnection();
private:
  struct ConnectionDetails {
    const char *server = D_SERVER;
    const char *database = DATABASE;
    const char *username = USERNAME;
    const char *password = PASSWORD;
  };

  MYSQL *connection {nullptr};
  void setupConnection();
};
#endif // DATABASE_HPP
