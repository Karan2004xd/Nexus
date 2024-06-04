#ifndef PRODUCER_HPP
#define PRODUCER_HPP

#include <cppkafka/configuration.h>
#include "../../constants.h"
#include <iostream>
#include <variant>
#include "../../dfs/include/Dfs.hpp"

using namespace Nexus;

class Producer : protected Dfs {
public:
  typedef std::unordered_map<std::string, std::variant<std::string, std::vector<std::string>>> RequestMap;
  Producer();
  void sendResponse(const std::string &requestData);
private:
  DfsResult getOperationResult(RequestMap &jsonData);
  std::string getJsonResposne(const std::string &requestData);
  cppkafka::Configuration producerConfig;
};
#endif // PRODUCER_HPP
