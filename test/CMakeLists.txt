cmake_minimum_required(VERSION 3.6)
project("testing")

add_executable(${PROJECT_NAME} "./main.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE "./include" "./src")
