#ifndef ROUTES_HPP
#define ROUTES_HPP

#include "OperationsHandler.hpp"
#include <crow/app.h>

class Routes {
public:
  void mapRoutes(crow::SimpleApp &app);

private:
  OperationsHandler operations;
  std::string currentUsername;

  void setCurrentUsername(const std::string &username);
  const std::string getCurrentUsername() const;
};
#endif //ROUTES_HPP
