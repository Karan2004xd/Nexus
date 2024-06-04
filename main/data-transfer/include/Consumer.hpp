#ifndef CONSUMER_HPP
#define CONSUMER_HPP

#include <cppkafka/configuration.h>
#include <cppkafka/consumer.h>
#include "Producer.hpp"

class Consumer {
public:
  Consumer();
  void startListeningForRequest();
private:
  cppkafka::Configuration consumerConfig;

  Producer producer;
};

#endif // CONSUMER_HPP
