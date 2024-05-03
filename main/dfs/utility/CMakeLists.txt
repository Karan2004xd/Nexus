cmake_minimum_required(VERSION 3.6)
project("utility")

add_library(${PROJECT_NAME}
  "./src/JsonDataHandler.cpp"
  "./src/JsonStringBuilder.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
