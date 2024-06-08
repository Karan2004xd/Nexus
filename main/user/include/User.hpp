#ifndef USER_HPP
#define USER_HPP

#include "../../dfs/include/Nexus.hpp"
#include "../include/UserOperations.hpp"

class Nexus::User {
protected:
  void addUser(const std::string &username, const std::string &password);
  void deleteUser(const std::string &username, const std::string &password);
  bool checkUser(const std::string &username, const std::string &password);
private:
  UserOperations userOperations;
};
#endif // USER_HPP
