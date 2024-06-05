#ifndef ROUTES_HPP
#define ROUTES_HPP

#include <crow/app.h>
#include "../../dfs/include/Dfs.hpp"

class Routes : protected Nexus::Dfs {
public:
  void mapRoutes(crow::SimpleApp &app);

private:
  std::string currentUsername;

  void setCurrentUsername(const std::string &username);
  const std::string getCurrentUsername() const;
};
#endif //ROUTES_HPP
