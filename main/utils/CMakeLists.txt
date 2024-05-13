cmake_minimum_required(VERSION 3.6)
project(utils)

add_library(${PROJECT_NAME}
  "./src/SimpleJsonParser.cpp"
  "./src/JsonParserBuilder.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
