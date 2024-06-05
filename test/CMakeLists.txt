cmake_minimum_required(VERSION 3.6)

project(test)

find_package(Crow)

add_executable(${PROJECT_NAME} "./app.cpp")
target_link_libraries(${PROJECT_NAME} PUBLIC Crow::Crow)
