project(hello)
cmake_minimum_required(VERSION 3.20)

ADD_LIBRARY(hello hello.cpp)

message(STATUS "@@test log")

target_link_libraries(hello -lpthread)
target_link_libraries(hello -lm)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

ADD_EXECUTABLE(test_hello main.cpp)
target_link_libraries(test_hello hello)
