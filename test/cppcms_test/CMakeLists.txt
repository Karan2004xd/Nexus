cmake_minimum_required(VERSION 3.6)
project("cppcms_test")

add_library(${PROJECT_NAME}
  "./src/BasicTest.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PRIVATE cppcms)
