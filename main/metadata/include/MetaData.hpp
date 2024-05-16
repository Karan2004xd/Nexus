#ifndef METADATA_HPP
#define METADATA_HPP

#include "DbOperHandler.hpp"
#include "../../dfs/include/Nexus.hpp"

class Nexus::MetaData : protected DbOperHandler {
public:
  const QueryResultMap getQueryDataMap(Nexus::Utils::SimpleQueryParser &queryParser);
  void updateData(Nexus::Utils::SimpleQueryParser &queryParser);
  void printData(DbOperHandler::QueryResultMap &data) const;

  MetaData() {
    checkConnection();
  }

  ~MetaData() {
    closeConnection();
  }
};
#endif // METADATA_HPP
