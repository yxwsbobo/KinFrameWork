//
// Created by Kin on 2018/9/22 0022.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "LuaEngine.h"
#include "../Lua/lua.hpp"

#include <iostream>

using LuaCoreType = lua_State;
static LuaCoreType * gtLua(std::any& anyLua)
{
    return std::any_cast<LuaCoreType*>(anyLua);
}

KinLua::LuaEngine::LuaEngine()
:Core{luaL_newstate()}
{
    luaL_openlibs(gtLua(Core));
}

KinLua::LuaEngine::~LuaEngine()
{
    if(Core.has_value())
    {
        lua_close(gtLua(Core));
    }
    Core = nullptr;
}

int KinLua::LuaEngine::Load(const std::string& FilePath)
{
    return luaL_dofile(gtLua(Core),FilePath.c_str());
}

int KinLua::LuaEngine::LoadCode(const std::string &Code)
{
    return luaL_dostring(gtLua(Core),Code.c_str());
}

KinLua::LuaVariable KinLua::LuaEngine::GetValue(const std::string &VarName)
{
    return KinLua::LuaVariable();
}

void KinLua::LuaEngine::LuaPushGlobalVariable(const std::string &Name)
{
    lua_getglobal(gtLua(Core),Name.c_str());
}

void KinLua::LuaEngine::LuaCall(int ArgNums,std::vector <LuaVariable> &ReturnValue)
{
    lua_call(gtLua(Core),ArgNums,ReturnValue.size());
    ReturnValue[0] = lua_tonumber(gtLua(Core),-1);
}

void KinLua::LuaEngine::LuaPushArg(const std::string &Arg)
{
    std::cout<<"in Push String :"<<Arg<<std::endl;
    lua_pushlstring(gtLua(Core),Arg.c_str(),Arg.size());
}

void KinLua::LuaEngine::LuaPushArg(int Number)
{
    std::cout<<"in PushInt:"<<Number<<std::endl;
    lua_pushinteger(gtLua(Core),Number);
}

void KinLua::LuaEngine::LuaPushArg(double Number)
{
    std::cout<<"in Push double :"<<Number<<std::endl;
    lua_pushnumber(gtLua(Core),Number);
}



