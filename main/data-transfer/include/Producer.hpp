#ifndef PRODUCER_HPP
#define PRODUCER_HPP

#include <cppkafka/configuration.h>
#include "../../constants.h"
#include <iostream>
#include "../../dfs/include/Dfs.hpp"
#include "../../utils/include/json/SimpleJsonParser.hpp"

using namespace Nexus;

class Producer : protected Dfs {
public:
  Producer();
  void sendResponse(const std::string &responseData);
  DfsResult getOperationResult(const Utils::SimpleJsonParser::JsonDataParams &jsonData);
private:
  cppkafka::Configuration producerConfig;
};
#endif // PRODUCER_HPP
