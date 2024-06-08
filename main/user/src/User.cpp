#include "../include/User.hpp"
using namespace Nexus;

void User::addUser(const std::string &username, const std::string &password) {
  userOperations.addUser(username, password);
}

void User::deleteUser(const std::string &username, const std::string &password) {
  userOperations.deleteUser(username, password);
}

bool User::checkUser(const std::string &username, const std::string &password) {
  return userOperations.checkUser(username, password);
}
