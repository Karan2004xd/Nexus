cmake_minimum_required(VERSION 3.6)

add_definitions(-std=c++17)

# find_program(EMSCRIPTEN emcc)
# if (EMSCRIPTEN)
#   message("Emscripten found: ${EMSCRIPTEN}")
#   set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

#   set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Os -s SIDE_MODULE=1 ")
#   set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS    "-Os -s WASM=1 -s SIDE_MODULE=1 -s STANDALONE_WASM --no-entry")

# else()
#   message(FATAL_ERROR "Emscripten not found. Please install Emscripten")
# endif()

project(app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("./dfs/server")
add_subdirectory("./dfs/dfs-api")
add_subdirectory("./dfs/data")
add_subdirectory("./dfs/utility")
add_subdirectory("./dfs/database")
add_subdirectory("./dfs/storage")

configure_file("./constants.h.in" ${CMAKE_SOURCE_DIR}/constants.h @ONLY)

add_executable(${PROJECT_NAME} "./app.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE dfs-api) 
