cmake_minimum_required(VERSION 3.6)
project(dfs)

set(OPERATION "operation" PARENT_SCOPE)
set(PUT_DATA "put-data" PARENT_SCOPE)
set(GET_DATA "get-data" PARENT_SCOPE)

set(GET_TRASH_DATA "get-trash-data" PARENT_SCOPE)
set(DELETE_DATA "delete-data" PARENT_SCOPE)

set(DELETE_TRASH_DATA "delete-trash-data" PARENT_SCOPE)
set(LIST_DATA "list-data" PARENT_SCOPE)


add_library(${PROJECT_NAME}
  "./src/Dfs.cpp"
  "./src/DfsResult.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PRIVATE metadata data storage)
