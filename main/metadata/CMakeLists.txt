cmake_minimum_required(VERSION 3.6)
project(metadata)

set(QUERIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/queries PARENT_SCOPE)
set(TEST_QUERIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/queries/test PARENT_SCOPE)

add_library(${PROJECT_NAME}
  "./src/DbConnHandler.cpp"
  "./src/DbOperHandler.cpp"
  "./src/MetaData.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PRIVATE utils mariadb)
