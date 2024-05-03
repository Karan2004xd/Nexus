cmake_minimum_required(VERSION 3.6)
project("storage")

set(SERVICE_COMPONENTS s3)
find_package(AWSSDK REQUIRED COMPONENTS ${SERVICE_COMPONENTS})

set(SECRET_ACCESS_KEY "SECRET_ACCESS_KEY" PARENT_SCOPE)
set(ACCESS_KEY_ID "ACCESS_KEY_ID" PARENT_SCOPE)

add_library(${PROJECT_NAME}
  "./src/AwsHandler.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME}
        ${AWSSDK_LINK_LIBRARIES})
