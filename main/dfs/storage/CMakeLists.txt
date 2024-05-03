cmake_minimum_required(VERSION 3.6)
project("storage")

find_package(AWSSDK REQUIRED COMPONENTS ${SERVICE_COMPONENTS})
set(SERVICE_COMPONENTS s3)

add_library(${PROJECT_NAME}
  "./src/ConnectorApi.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME}
        ${AWSSDK_LINK_LIBRARIES})
