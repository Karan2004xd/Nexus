#include "../include/MetaData.hpp"
#include "../../utils/include/query/SimpleQueryParser.hpp"
#include <iostream>

using namespace Nexus;

void MetaData::printData(DbOperHandler::QueryResultMap &data) const {
  std::vector<std::vector<std::string>> row;
  for (const auto &field : data) {
    std::cout << field.first << "\t";
    row.push_back(field.second);
  }
  std::cout << "\n";

  for (int j = 0; j < row[0].size(); j++) {
    for (int i = 0; i < row.size(); i++) {
      std::cout << row[i][j] << "\t";
    }
    std::cout << "\n";
  }
}

void MetaData::updateData(Nexus::Utils::SimpleQueryParser &queryParser) {
  /* checkConnection(); */
  runQuery(queryParser.getParsedData());
  std::cout << "Data updated successfully" << std::endl;
}

const MetaData::QueryResultMap MetaData::getQueryDataMap(Utils::SimpleQueryParser &queryParser) {
  /* checkConnection(); */
  return getData(queryParser.getParsedData());
}

const std::vector<std::string> MetaData::getFieldNames(Utils::SimpleQueryParser &queryParser) {
  std::vector<std::string> result;
  auto queryOutput = getQueryDataMap(queryParser);
  for (const auto &key : queryOutput) {
    result.push_back(key.first);
  }
  return result;
}
