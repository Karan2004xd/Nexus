cmake_minimum_required(VERSION 3.6)
project(data)

add_library(${PROJECT_NAME}
  "./src/Data.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
