#include "../include/UserOperations.hpp"
#include "../../utils/include/json/SimpleJsonParser.hpp"
#include "../../utils/include/query/SimpleQueryParser.hpp"
#include "../../constants.h"
#include <stdexcept>

using namespace Nexus;

bool UserOperations::checkUser(const std::string &username, const std::string &password) {
  auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
    .singleData("file", "CheckUser")
    .singleData("username", username)
    .singleData("password", password)
    .getJsonData();

  auto queryData = Utils::SimpleQueryParser::parseQuery(USER_QUERIES_DIR, jsonData);
  auto queryOutput = metaData.getQueryDataMap(queryData);
  return queryOutput.size() > 0;
}

void UserOperations::addUser(const std::string &username, const std::string &password) {
  if (!checkUser(username, password)) {
    auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
      .singleData("file", "AddUser")
      .singleData("username", username)
      .singleData("password", password)
      .getJsonData();

    auto queryData = Utils::SimpleQueryParser::parseQuery(USER_QUERIES_DIR, jsonData);
    metaData.updateData(queryData);
  } else {
    throw new std::runtime_error("The User already exists");
  }
}

void UserOperations::deleteUser(const std::string &username, const std::string &password) {
  if (checkUser(username, password)) {
    auto jsonData = Utils::SimpleJsonParser::JsonBuilder()
      .singleData("file", "DeleteUser")
      .singleData("username", username)
      .singleData("password", password)
      .getJsonData();

    auto queryData = Utils::SimpleQueryParser::parseQuery(USER_QUERIES_DIR, jsonData);
    metaData.updateData(queryData);
  } else {
    throw new std::runtime_error("The User doesn't exist");
  }
}
