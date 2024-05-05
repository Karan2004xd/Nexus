cmake_minimum_required(VERSION 3.6)
project(storage)

set(SERVICE_COMPONENTS s3)
find_package(AWSSDK REQUIRED COMPONENTS ${SERVICE_COMPONENTS})

set(BUCKET_MAIN_AP_SOUTH_1 "main-ap-south-1" PARENT_SCOPE)
set(BUCKET_MAIN_AP_SOUTHEAST_1 "main-ap-southeast-1" PARENT_SCOPE)
set(BUCKET_MAIN_US_EAST_1_BUCKET "main-us-east-1-bucket" PARENT_SCOPE)
set(BUCKET_MAIN_US_EAST_2_BUCKET "main-us-east-2-bucket" PARENT_SCOPE)
set(BUCKET_MAIN_EU_NORTH_1 "main-eu-north-1" PARENT_SCOPE)

set(BUCKET_BACKUP_EU_WEST_2 "backup-eu-west-2" PARENT_SCOPE)

add_library(${PROJECT_NAME}
  "./src/AwsHandler.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PRIVATE ${AWSSDK_LINK_LIBRARIES})
