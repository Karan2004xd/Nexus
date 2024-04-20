cmake_minimum_required(VERSION 3.6)
project(app)

add_subdirectory("./dfs/server")
add_subdirectory("./dfs/data")
configure_file("./constants.h.in" ${CMAKE_SOURCE_DIR}/constants.h @ONLY)

add_executable(${PROJECT_NAME} "./app.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE server)
