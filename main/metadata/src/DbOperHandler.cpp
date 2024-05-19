#include "../include/DbOperHandler.hpp"
#include <iostream>
#include <stdexcept>

MYSQL_RES *DbOperHandler::executeQuery(const std::string &query) {
  int runQuery = mysql_query(connection, query.c_str());
  MYSQL_RES *result {nullptr};
  if (runQuery) {
    throw std::runtime_error("(DbOperHandler) : Error while executing query, Error : "
                             + std::string(mysql_error(connection)));
  } else {
    result = mysql_use_result(connection);
  }
  return result;
}

void DbOperHandler::runQuery(const std::string &query) {
  executeQuery(query);
}

std::vector<std::string> DbOperHandler::getFieldNames(MYSQL_RES *res) {
  std::vector<std::string> resultVec;
  MYSQL_FIELD *field;

  field = mysql_fetch_fields(res);
  for (int i = 0; i < mysql_num_fields(res); i++) {
    resultVec.push_back(field[i].name);
  }
  return resultVec;
}

DbOperHandler::QueryResultMap DbOperHandler::getData(const std::string &query) {
  QueryResultMap result;

  MYSQL_ROW row;
  MYSQL_RES *queryResult = executeQuery(query);

  auto fields = getFieldNames(queryResult);

  while ((row = mysql_fetch_row(queryResult)) != NULL) {
    for (int i = 0; i < mysql_num_fields(queryResult); i++) {
      std::string rowValue;
      if (row[i] == NULL) {
        rowValue = "NULL";
      } else {
        rowValue = row[i];
      }
      if (result.find(fields[i]) != result.end()) {
        result[fields[i]].push_back(rowValue);
      } else {
        result[fields[i]] = {rowValue};
      }
    }
  }

  delete queryResult;
  return result;
}
