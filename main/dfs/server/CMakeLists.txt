cmake_minimum_required(VERSION 3.6)
project(server)

find_package(Boost REQUIRED COMPONENTS log log_setup)

include_directories(${Boost_INCLUDE_DIRS})

set(DEFAULT_PORT "8082" PARENT_SCOPE)
set(DEFAULT_HOST "localhost" PARENT_SCOPE)

set(PATH_TO_INTERFACES ${CMAKE_SOURCE_DIR}/dfs/interface PARENT_SCOPE)
set(PATH_TO_ASSETS ${CMAKE_SOURCE_DIR}/dfs/interface/assets PARENT_SCOPE)
set(PATH_TO_STYLES ${CMAKE_SOURCE_DIR}/dfs/interface/styles PARENT_SCOPE)

add_library(${PROJECT_NAME}
  "./src/ServerHandler.cpp"
  "./src/Server.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::log Boost::log_setup dfs-api)
