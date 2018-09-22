cmake_minimum_required(VERSION 3.12)
project(KinLua)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{KinBase})
include(KinCommonSetting)

add_subdirectory(Source)
add_subdirectory(Examples)

