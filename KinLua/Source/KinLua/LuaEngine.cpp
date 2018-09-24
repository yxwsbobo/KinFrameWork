//
// Created by Kin on 2018/9/22 0022.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "LuaEngine.h"
#include "../Lua/lua.hpp"

#include <iostream>

static void LuaCoreDeleter(lua_State *e)
{
    try
    {
        if(e != nullptr)
        {
            lua_close(e);
        }
    }
    catch(...)
    {

    }
}

KinLua::LuaEngine::LuaEngine()
        : Core{luaL_newstate(), LuaCoreDeleter}
{
    luaL_openlibs(Core.get());
}

int KinLua::LuaEngine::Run(const std::string &FilePath)
{
    return luaL_dofile(Core.get(), FilePath.c_str());
}

int KinLua::LuaEngine::RunCode(const std::string &Code)
{
    return luaL_dostring(Core.get(), Code.c_str());
}
