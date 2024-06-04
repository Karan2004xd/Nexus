#ifndef KAFKA_API_HPP
#define KAFKA_API_HPP

#include "Consumer.hpp"
class KafkaApi {
public:
  void startKafkaApi() {
    consumer.startListeningForRequest();
  };
private:
  Consumer consumer;
};

#endif // KAFKA_API_HPP
