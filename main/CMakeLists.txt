cmake_minimum_required(VERSION 3.6)

project(app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("./dfs/server")
add_subdirectory("./dfs/dfs-api")
add_subdirectory("./dfs/data")
add_subdirectory("./dfs/utility")
add_subdirectory("./dfs/database")
add_subdirectory("./dfs/storage")

configure_file("./constants.h.in" ${CMAKE_SOURCE_DIR}/constants.h @ONLY)

add_executable(${PROJECT_NAME} "./app.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE server) 
