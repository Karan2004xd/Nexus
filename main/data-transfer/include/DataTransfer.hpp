#ifndef DATA_TRANSFER_HPP
#define DATA_TRANSFER_HPP
#include "../../dfs/include/Nexus.hpp"
#include "Consumer.hpp"

using namespace Nexus;

class Nexus::DataTransfer {
public:
  void startKafkaApi() {
    consumer.startListeningForRequest();
  };
private:
  Consumer consumer;
};

#endif // DATA_TRANSFER_HPP
