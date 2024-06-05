cmake_minimum_required(VERSION 3.6)

project(server)
find_package(Crow)

set(DEFAULT_PORT "8082" PARENT_SCOPE)
set(DEFAULT_HOST "localhost" PARENT_SCOPE)
set(CLIENT_USERNAME "username" PARENT_SCOPE)
set(CLIENT_PASSWORD "password" PARENT_SCOPE)

add_library(${PROJECT_NAME}
  "./src/Routes.cpp"
  "./src/ServerHandler.cpp"
  "./src/OperationsHandler.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PUBLIC Crow::Crow dfs utils)
