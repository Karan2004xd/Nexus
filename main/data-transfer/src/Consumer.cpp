#include "../include/Consumer.hpp"
#include <future>

Consumer::Consumer() {
  this->consumerConfig = {
    {"bootstrap.servers", KAFKA_BROKER},
    {"group.id", KAFKA_NEXUS_BACKEND_GROUP_ID},
    {"session.timeout.ms", 6000},
    {"max.poll.interval.ms", 60000}
  };
}

void Consumer::startListeningForRequest() {
  try {
    cppkafka::Consumer consumer {consumerConfig};
    consumer.subscribe({KAFKA_NEXUS_BACKEND_TOPIC});
    consumer.set_timeout(std::chrono::milliseconds {100});

    while (true) {
      auto msg = consumer.poll();
      if (msg) {
        if (msg.get_error()) {
          continue;
        } else {
          std::cout << "Consumer data : " << msg.get_payload() << std::endl;
          std::future<void> responseFuture = std::async(
            [this, msg = std::move(msg)]() mutable {
              producer.sendResponse(msg.get_payload());
            }
          );
        }
        consumer.async_commit();
      } 
    }
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}
