#ifndef USER_OPERATIONS_HPP
#define USER_OPERATIONS_HPP
#include "../../metadata/include/MetaData.hpp"

class UserOperations {
public:
  void addUser(const std::string &username, const std::string &password);
  void deleteUser(const std::string &username, const std::string &password);
  bool checkUser(const std::string &username, const std::string &password);
private:
  Nexus::MetaData metaData;
};
#endif // USER_OPERATIONS_HPP
