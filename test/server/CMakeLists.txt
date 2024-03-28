cmake_minimum_required(VERSION 3.6)
project(server)

add_library(${PROJECT_NAME}
  "./src/ServerHandler.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
