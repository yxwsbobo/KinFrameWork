

set(KinLua_INCLUDE_DIRS $ENV{KinBase}/Lua)
set(KinLua_LIBRARIES_PATH $ENV{KinBase}/Lua/bin/${CMAKE_CXX_COMPILER_ID}/${CMAKE_BUILD_TYPE})
set(KinLua_LIBRARIES KinLua LuaCore)

if(KIN_EASY_USE_LIB)
include_directories(${KinLua_INCLUDE_DIRS})
link_directories(${KinLua_LIBRARIES_PATH})
endif(KIN_EASY_USE_LIB)
