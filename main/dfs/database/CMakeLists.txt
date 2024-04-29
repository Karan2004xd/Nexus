cmake_minimum_required(VERSION 3.6)
project(database)

add_library(${PROJECT_NAME}
  "./src/DatabaseHandler.cpp"
  "./src/Metadata.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include" "/usr/include/mysql")
target_link_libraries(${PROJECT_NAME} PRIVATE mariadb)
