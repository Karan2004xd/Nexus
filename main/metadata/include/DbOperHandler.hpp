#ifndef DB_OPER_HANDLER_HPP
#define DB_OPER_HANDLER_HPP

#include "DbConnHandler.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class DbOperHandler : protected DbConnHandler {
protected:
  typedef std::unordered_map<std::string, std::vector<std::string>> QueryResultMap;

  void runQuery(const std::string &query);
  QueryResultMap getData(const std::string &query);
  std::vector<std::string> getFieldNames(MYSQL_RES *res);
private:
  MYSQL_RES *executeQuery(const std::string &query);
};
#endif // DB_OPER_HANDLER_HPP
