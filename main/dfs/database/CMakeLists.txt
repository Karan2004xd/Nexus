cmake_minimum_required(VERSION 3.6)
project(database)

set(QUERIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/queries PARENT_SCOPE)

add_library(${PROJECT_NAME}
  "./src/QueryParser.cpp"
  "./src/DatabaseHandler.cpp"
  "./src/Metadata.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include" "/usr/include/mysql")
target_link_libraries(${PROJECT_NAME} PRIVATE mariadb)
