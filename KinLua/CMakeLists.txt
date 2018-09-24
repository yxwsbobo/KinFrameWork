cmake_minimum_required(VERSION 3.12)
project(KinLua)

set(CMAKE_CXX_STANDARD 17)

find_path(KinFrameworkPath KinCommonSetting.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${KinFrameworkPath})
include(KinCommonSetting)

find_package(KinBase)

add_subdirectory(Source)
add_subdirectory(Examples)

