project(hello)
cmake_minimum_required(VERSION 2.8)

ADD_LIBRARY(hello hello.cpp)

message(STATUS "@@test log")

target_link_libraries(hello -lpthread)
target_link_libraries(hello -lm)

ADD_EXECUTABLE(test_hello main.cpp)
target_link_libraries(test_hello hello)
