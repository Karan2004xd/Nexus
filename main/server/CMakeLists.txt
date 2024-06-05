cmake_minimum_required(VERSION 3.6)

project(server)
find_package(Crow)

add_library(${PROJECT_NAME}
  "./src/Routes.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PUBLIC Crow::Crow dfs)
