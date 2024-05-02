#include "../include/DatabaseHandler.hpp"
#include "../include/JsonStringBuilder.hpp"
#include <iostream>

using namespace Database;

void DatabaseHandler::checkConnection() {
  if (connection == nullptr) {
    mysqlConnectionSetup();
  }
}

void DatabaseHandler::mysqlConnectionSetup() { 
  connection = mysql_init(NULL);
  struct ConnectionDetails conn_details;
  if (!mysql_real_connect(connection,
                          conn_details.server,
                          conn_details.username,
                          conn_details.password,
                          conn_details.database, 0, NULL, 0)) {
    throw std::runtime_error("Connection error: " + std::string(mysql_error(connection)));
  } else {
    std::cout << "Connected to database" << std::endl;
  }
}

MYSQL_RES *DatabaseHandler::executeQuery(const std::string &sqlQuery) {
  int runQuery = mysql_query(connection, sqlQuery.c_str());
  MYSQL_RES *result {nullptr};
  if (runQuery) {
    throw std::runtime_error("Mysql Query Error: " + std::string(mysql_error(connection)) +
                             "\nQuery: " + sqlQuery);
  } else {
    result = mysql_use_result(connection);
  }
  return result;
}

void DatabaseHandler::updateData(const JsonStringBuilder &builder) {
  setQuery(builder);
  executeQuery(query);

  std::cout << "\nData Updated Successfully" << std::endl;
}

void DatabaseHandler::printData(std::unordered_map<int, std::vector<std::string>> &data) {
  if (data.size() > 0) {
    std::cout << "\nPrinting data...\n" << std::endl;
    for (const auto &dataStream : data) {
      for (const auto &data : dataStream.second) {
        std::cout << data << "\t";
      }
      std::cout << "\n";
    }
  } else {
    std::cout << "The data set provided is empty" << std::endl;
  }
}

std::unordered_map<int, std::vector<std::string>> DatabaseHandler::getDataByRow(const JsonStringBuilder &builder) {
  setQuery(builder);

  MYSQL_ROW row;
  MYSQL_RES *result = executeQuery(query);

  std::unordered_map<int, std::vector<std::string>> rowData;
  int rowNum = 1;

  while ((row = mysql_fetch_row(result)) != NULL) {
    std::vector<std::string> fieldsData;
    for (int i = 0; i < mysql_num_fields(result); i++) {
      fieldsData.push_back(row[i]);
    }
    rowData[rowNum] = fieldsData;
    rowNum++;
  }
  return rowData;
}

std::unordered_map<int, std::vector<std::string>> DatabaseHandler::getDataByColumn(const JsonStringBuilder &builder) {
  setQuery(builder);

  MYSQL_ROW row;
  MYSQL_RES *result = executeQuery(query);
  
  std::unordered_map<int, std::vector<std::string>> columnData;
  
  while ((row = mysql_fetch_row(result)) != NULL) {
    int columnNum = 1;
    for (int i = 0; i < mysql_num_fields(result); i++) {
      if (columnData.find(columnNum) == columnData.end()) {
        columnData[columnNum] = std::vector<std::string> {row[i]};
      } else {
        columnData[columnNum].push_back(row[i]);
      }
      columnNum++;
    }
  }
  return columnData;
}

void DatabaseHandler::setQuery(const JsonStringBuilder &builder) {
  std::string rawJsonData = builder.str();

  QueryParser::parseJsonData(rawJsonData);
  this->query = QueryParser::getParsedQuery();
}

DatabaseHandler::DatabaseHandler() {
  checkConnection();
}

DatabaseHandler::~DatabaseHandler() {
  mysql_close(connection);
}
