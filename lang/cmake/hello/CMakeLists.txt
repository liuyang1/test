project(hello)
cmake_minimum_required(VERSION 3.20)

add_library(hello hello.cpp hello.cpp)
target_sources(hello PRIVATE hello.cpp)

message(STATUS "@@test log")

target_link_libraries(hello -lpthread)
target_link_libraries(hello -lm)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(test_hello main.cpp main.cpp)
target_link_libraries(test_hello hello)
