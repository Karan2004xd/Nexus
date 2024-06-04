cmake_minimum_required(VERSION 3.6)
project(data-transfer)

set(KAFKA_BROKER "localhost:9092" PARENT_SCOPE)
set(KAFKA_NEXUS_BACKEND_TOPIC "nexus-backend-topic" PARENT_SCOPE)
set(KAFKA_NEXUS_BACKEND_GROUP_ID "nexus-backend-group" PARENT_SCOPE)
set(KAFKA_NEXUS_FRONTEND_GROUP_ID "nexus-frontend-group" PARENT_SCOPE)
set(KAFKA_NEXUS_FRONTEND_TOPIC "nexus-frontend-topic" PARENT_SCOPE)

find_package(Boost REQUIRED COMPONENTS log log_setup)
include_directories(${Boost_INCLUDE_DIRS})

add_library(${PROJECT_NAME}
  "./src/Consumer.cpp"
  "./src/Producer.cpp"
  "./src/DataTransfer.cpp"
  "./src/Logging.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PRIVATE cppkafka Boost::log Boost::log_setup dfs)
