cmake_minimum_required(VERSION 3.6)
project(storage)

set(SERVICE_COMPONENTS s3)
find_package(AWSSDK REQUIRED COMPONENTS ${SERVICE_COMPONENTS})

add_library(${PROJECT_NAME}
  "./src/AwsS3.cpp"
  "./src/AwsCrudOperHandler.cpp"
  "./src/AwsS3SetupHandler.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} ${AWSSDK_LINK_LIBRARIES} utils metadata)
