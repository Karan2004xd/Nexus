cmake_minimum_required(VERSION 3.6)
project(dfs)

add_library(${PROJECT_NAME}
  "./src/Dfs.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
