//
// Created by Kin on 2018/9/22 0022.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "LuaEngine.h"
#include "../Lua/lua.hpp"

#include <iostream>

namespace KinLua
{
    using Lua = lua_State*;
}

KinLua::LuaEngine::LuaEngine()
        : Core{luaL_newstate()}
{
    luaL_openlibs(Lua(Core));
}

KinLua::LuaEngine::~LuaEngine()
{
    if(Core != nullptr)
    {
        lua_close(Lua(Core));
    }
    Core = nullptr;
}

int KinLua::LuaEngine::Load(const std::string &FilePath)
{
    return luaL_dofile(Lua(Core), FilePath.c_str());
}

int KinLua::LuaEngine::LoadCode(const std::string &Code)
{
    return luaL_dostring(Lua(Core), Code.c_str());
}

KinLua::LuaVariable KinLua::LuaEngine::operator[](const std::string &Name)
{
    auto Type = lua_getglobal(Lua(Core),Name.c_str());
    return KinLua::LuaVariable();
}

void KinLua::LuaEngine::LuaPushGlobalVariable(const std::string &Name)
{
    lua_getglobal(Lua(Core), Name.c_str());
}

void KinLua::LuaEngine::LuaCall(int ArgNums, std::vector <LuaVariable> &ReturnValue)
{
    lua_call(Lua(Core), ArgNums, ReturnValue.size());
    ReturnValue[0] = lua_tonumber(Lua(Core), -1);
}

void KinLua::LuaEngine::LuaPushArg(const std::string &Arg)
{
    std::cout << "in Push String :" << Arg << std::endl;
    lua_pushlstring(Lua(Core), Arg.c_str(), Arg.size());
}

void KinLua::LuaEngine::LuaPushArg(int Number)
{
    std::cout << "in PushInt:" << Number << std::endl;
    lua_pushinteger(Lua(Core), Number);
}

void KinLua::LuaEngine::LuaPushArg(double Number)
{
    std::cout << "in Push double :" << Number << std::endl;
    lua_pushnumber(Lua(Core), Number);
}

