cmake_minimum_required(VERSION 3.6)
project(utils)

add_library(${PROJECT_NAME}
  "./src/json/SimpleJsonParser.cpp"
  "./src/json/JsonParserBuilder.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
