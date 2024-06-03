cmake_minimum_required(VERSION 3.6)
project(user)

add_library(${PROJECT_NAME}
  "./src/UserOperations.cpp"
  "./src/User.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PRIVATE utils metadata)
