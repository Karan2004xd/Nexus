#ifndef METADATA_HPP
#define METADATA_HPP

#include "DbOperHandler.hpp"
#include "../../dfs/include/Nexus.hpp"

class Nexus::MetaData : protected DbOperHandler {
public:
  typedef std::unordered_map<std::string, std::vector<std::string>> QueryResultMap;
  const QueryResultMap getQueryDataMap(Nexus::Utils::SimpleQueryParser &queryParser);
  void updateData(Nexus::Utils::SimpleQueryParser &queryParser);

  void printData(DbOperHandler::QueryResultMap &data) const;
  const std::vector<std::string> getFieldNames(Utils::SimpleQueryParser &queryParser);

  MetaData() {
    checkConnection();
  }

  ~MetaData() {
    closeConnection();
  }
};
#endif // METADATA_HPP
