cmake_minimum_required(VERSION 3.6)
project(nexus)

add_subdirectory("./server")

add_executable(${PROJECT_NAME} "./app.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE server)
