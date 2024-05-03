# Set the minimum required version of CMake for this project.
cmake_minimum_required(VERSION 3.13)

set(SERVICE_COMPONENTS s3)
project("aws")

set(CMAKE_CXX_STANDARD 11)

find_package(AWSSDK REQUIRED COMPONENTS ${SERVICE_COMPONENTS})

add_executable(${PROJECT_NAME}
        helloAWS.cpp)

target_link_libraries(${PROJECT_NAME}
        ${AWSSDK_LINK_LIBRARIES})

