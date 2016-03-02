cmake_minimum_required(VERSION 2.8)
project(http_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)

file(MAKE_DIRECTORY /home/box/final)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/box/final )
add_executable(final  ${SOURCE_FILES})