cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

add_executable(luac luac.c)
add_executable(lua lua.c)
target_link_libraries(luac LuaCore)
target_link_libraries(lua LuaCore)

add_library(LuaCore
        lapi.c
        lapi.h
        lauxlib.c
        lauxlib.h
        lbaselib.c
        lbitlib.c
        lcode.c
        lcode.h
        lcorolib.c
        lctype.c
        lctype.h
        ldblib.c
        ldebug.c
        ldebug.h
        ldo.c
        ldo.h
        ldump.c
        lfunc.c
        lfunc.h
        lgc.c
        lgc.h
        linit.c
        liolib.c
        llex.c
        llex.h
        llimits.h
        lmathlib.c
        lmem.c
        lmem.h
        loadlib.c
        lobject.c
        lobject.h
        lopcodes.c
        lopcodes.h
        loslib.c
        lparser.c
        lparser.h
        lprefix.h
        lstate.c
        lstate.h
        lstring.c
        lstring.h
        lstrlib.c
        ltable.c
        ltable.h
        ltablib.c
        ltm.c
        ltm.h
        lua.h
        lua.hpp
        luaconf.h
        lualib.h
        lundump.c
        lundump.h
        lutf8lib.c
        lvm.c
        lvm.h
        lzio.c
        lzio.h)

