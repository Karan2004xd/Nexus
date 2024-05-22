cmake_minimum_required(VERSION 3.6)
project(dfs)

add_library(${PROJECT_NAME}
  "./src/Dfs.cpp"
  "./src/DfsResult.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PRIVATE metadata data storage)
