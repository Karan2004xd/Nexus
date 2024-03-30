cmake_minimum_required(VERSION 3.6)
project(server)

set(PATH_TO_SOURCES_FOLDER "${CMAKE_SOURCE_DIR}/sources/" PARENT_SCOPE)

add_library(${PROJECT_NAME}
  "./src/ServerHandler.cpp"
  "./src/Server.cpp"
  "./src/SourcesHandler.cpp"
  "./src/DataHandler.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
