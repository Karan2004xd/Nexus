cmake_minimum_required(VERSION 3.6)
project("dfs-api")

add_library(${PROJECT_NAME}
  "./src/Dfs.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PRIVATE server data utility)
