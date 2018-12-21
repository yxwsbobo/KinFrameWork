cmake_minimum_required(VERSION 3.12)

project(Example)

set(CMAKE_CXX_STANDARD 17)

find_package(KinBase)
find_package(catch2)

add_executable(Example main.cpp TestClass.cpp TestClass.h ObjectTest.cpp TestCircleBuffer.cpp TestCircleBuffer.h)
add_library(MyTestLib MyTestLib.cpp MyTestLib.h)

target_link_libraries(Example MyTestLib)
