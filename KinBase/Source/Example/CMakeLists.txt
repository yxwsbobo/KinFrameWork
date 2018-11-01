cmake_minimum_required(VERSION 3.12)

project(Example)

set(CMAKE_CXX_STANDARD 17)

find_package(KinBase)

add_executable(Example main.cpp TestClass.cpp TestClass.h)
add_library(MyTestLib MyTestLib.cpp MyTestLib.h)

target_link_libraries(Example MyTestLib ${KinAllLibs})