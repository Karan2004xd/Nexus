cmake_minimum_required(VERSION 3.6)
project(data)

set(FILE_NAME "filename" PARENT_SCOPE)
set(FILE_CONTENT "content" PARENT_SCOPE)

add_library(${PROJECT_NAME}
  "./src/Contents.cpp"
  "./src/DataEncryptor.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PRIVATE utils)
