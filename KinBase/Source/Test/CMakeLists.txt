cmake_minimum_required(VERSION 3.12)

project(Test)

set(CMAKE_CXX_STANDARD 17)

find_package(KinBase)
find_package(catch2)

add_executable(Test main.cpp TestException.cpp TestLog.cpp)