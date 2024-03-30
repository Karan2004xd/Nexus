cmake_minimum_required(VERSION 3.6)
project("testing")

add_subdirectory("./server")

configure_file("./constants.h.in" ${CMAKE_SOURCE_DIR}/constants.h @ONLY)

add_executable(${PROJECT_NAME} "./main.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE server)
