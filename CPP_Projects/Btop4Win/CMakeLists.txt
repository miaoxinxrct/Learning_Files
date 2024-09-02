cmake_minimum_required(VERSION 3.5.0)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/debug")

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR/debug})


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGES} -g")

project(Btop4Win)

include_directories(include)

aux_source_directory(src SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})