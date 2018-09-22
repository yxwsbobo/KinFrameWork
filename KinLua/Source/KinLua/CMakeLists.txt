cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

add_library(KinLua LuaEnumDefine.h LuaVariable.cpp LuaVariable.h LuaEngine.cpp LuaEngine.h)

#target_link_libraries(KinLua LuaCore)