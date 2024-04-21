cmake_minimum_required(VERSION 3.6)
project(data)

set(FORM_BOUNDRY "------WebKitFormBoundary" PARENT_SCOPE)
set(FILENAME "filename" PARENT_SCOPE)
set(CONTENT_TYPE "Content-Type" PARENT_SCOPE)

add_library(${PROJECT_NAME}
  "./src/Data.cpp"
  "./src/DataChunker.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
