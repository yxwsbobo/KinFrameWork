//
// Created by Kin on 2018/9/22 0022.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "Lua.h"
#include "../Lua/lua.hpp"

KinLua::Lua::Lua()
{
    auto state = luaL_newstate();
    Core.reset(state,[](LuaCoreType* core){
        if(core != nullptr)
        {
            lua_close(core);
        }

    });
    
    luaL_openlibs(Core.get());
}

KinLua::Lua::~Lua()
{

}

KinLua::ResultEnum KinLua::Lua::RunFile(std::string Path)
{
    return (ResultEnum)luaL_dofile(Core.get(),Path.c_str());
}

KinLua::ResultEnum KinLua::Lua::RunCode(std::string Code)
{
    return (ResultEnum)luaL_dostring(Core.get(),Code.c_str());
}
