cmake_minimum_required(VERSION 3.6)
project(server)

find_package(Boost REQUIRED COMPONENTS log log_setup)
include_directories(${Boost_INCLUDE_DIRS})

set(DEFAULT_PORT "8080" PARENT_SCOPE)
set(DEFAULT_HOST "localhost" PARENT_SCOPE)
set(POST "post" PARENT_SCOPE)
set(GET "get" PARENT_SCOPE)

add_library(${PROJECT_NAME}
  "./src/Server.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::log Boost::log_setup)
