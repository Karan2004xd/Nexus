cmake_minimum_required(VERSION 3.6)
project(utils)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PRIVATE rapidjson)
