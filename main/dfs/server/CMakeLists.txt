cmake_minimum_required(VERSION 3.6)
project(server)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(DEFAULT_PORT "8080" PARENT_SCOPE)
set(DEFAULT_HOST "localhost" PARENT_SCOPE)

add_library(${PROJECT_NAME}
  "./src/ServerHandler.cpp"
  "./src/Server.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
