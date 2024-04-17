cmake_minimum_required(VERSION 3.6)
project(server)

find_package(Boost REQUIRED COMPONENTS system log asio beast)
include_directories(${Boost_INCLUDE_DIRS})

add_library(${PROJECT_NAME}

)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PRIVATE boost)
