cmake_minimum_required(VERSION 3.5)
project(OSExp1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c TypeDefines.h QueueHandler.h QueueHandler.c)
add_executable(OSExp1 ${SOURCE_FILES})