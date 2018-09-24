#include <utility>

//
// Created by Kin on 2018/9/22 0022.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "LuaVariable.h"

#include "../Lua/lua.hpp"


KinLua::LuaVariable::LuaVariable(const std::weak_ptr <KinLua::LuaCoreType> &Core, int Index)
        : Core(Core), Index(Index)
{
}

KinLua::LuaVariable::~LuaVariable()
{
    auto Lua = GetCore();
    auto End = lua_gettop(Lua.get());

    Must(End != 0);

    if(End == Index)
    {
        lua_pop(Lua.get(), 1);
    }
    else
    {
        //Todo Need to think
    }


//    auto Type = GetType();
//
//
//    switch (Type)
//    {
//        case LuaValueType::Boolean:
//        case LuaValueType::Number:
//        case LuaValueType::String:
//        {
//            lua_pop(Lua.get(),1);
//            break;
//        }
//        default:
//            break;
//    }

}

KinLua::LuaVariable KinLua::LuaVariable::CreateFromIndex(int Index)
{
    auto Lua = GetCore();

    if(Index < 0)
    {
        Index = lua_absindex(Lua.get(), Index);
    }

    return KinLua::LuaVariable{Core, Index};
}

KinLua::LuaValueType KinLua::LuaVariable::GetType()
{
    auto Lua = GetCore();
    if(!Index)
    {
        return LuaValueType::Environment;
    }
    return LuaValueType::_from_integral(lua_type(Lua.get(), Index));
}


KinLua::LuaVariable KinLua::LuaVariable::operator[](const std::string& Name)
{
    auto Lua = GetCore();

    auto Type = GetType();

    switch (Type)
    {
        case LuaValueType::Environment:
        {
            MakeSpaceValid(1);
            lua_getglobal(Lua.get(), Name.c_str());
            return CreateFromIndex(-1);
        }

        default:
        {
            Must(0, "UnValid Access Use Type :", Type);
        }
    }
    return LuaVariable{Core};
}

KinLua::LuaVariable::operator bool()
{
    auto Lua = GetCore();
    Must(lua_isboolean(Lua.get(), Index));

    auto Result = lua_toboolean(Lua.get(), Index);

    return static_cast<bool>(Result);
}

KinLua::LuaVariable::operator double()
{
    auto Lua = GetCore();

    Must(lua_isnumber(Lua.get(), Index));

    auto Result = lua_tonumber(Lua.get(), Index);

    return Result;
}

KinLua::LuaVariable::operator long long()
{
    auto Lua = GetCore();

    Must(lua_isinteger(Lua.get(), Index));

    auto Result = lua_tointeger(Lua.get(), Index);

    return Result;
}

KinLua::LuaVariable::operator std::string()
{
    auto Lua = GetCore();

    Must(lua_isstring(Lua.get(), Index));

    auto Result = lua_tostring(Lua.get(), Index);

    return Result;
}

bool KinLua::LuaVariable::operator=(bool Value)
{
    return false;
}

double KinLua::LuaVariable::operator=(double Value)
{
    return 0;
}

long long KinLua::LuaVariable::operator=(long long Value)
{
    return 0;
}

std::string KinLua::LuaVariable::operator=(const std::string &Value)
{
    return "";
}

std::vector <KinLua::LuaVariable> KinLua::LuaVariable::LuaCall(int Size)
{
    auto Lua = GetCore();

    auto Begin = lua_gettop(Lua.get()) - Size - 1;
    lua_call(Lua.get(), Size, LUA_MULTRET);
    auto End = lua_gettop(Lua.get());

    Must(End > Begin, "UnBelievable things happened!");

    std::vector <KinLua::LuaVariable> Result(std::vector <LuaVariable>::size_type(End - Begin), Core);

    for(auto &&result : Result)
    {
        result.Index = ++Begin;
    }

    return std::move(Result);
}

void KinLua::LuaVariable::MakeSpaceValid(int Size)
{
    auto Lua = GetCore();
    Should(lua_checkstack(Lua.get(), Size));
}

void KinLua::LuaVariable::PushSelf()
{
    auto Lua = GetCore();
    lua_pushvalue(Lua.get(), Index);
}

void KinLua::LuaVariable::PushValue(bool Value)
{
    auto Lua = GetCore();
    lua_pushboolean(Lua.get(), Value);
}

void KinLua::LuaVariable::PushValue(double Value)
{
    auto Lua = GetCore();
    lua_pushnumber(Lua.get(), Value);
}

void KinLua::LuaVariable::PushValue(long long Value)
{
    auto Lua = GetCore();
    lua_pushinteger(Lua.get(), Value);
}


void KinLua::LuaVariable::PushValue(const std::string &Value)
{
    auto Lua = GetCore();
    lua_pushlstring(Lua.get(), Value.c_str(), Value.size());
}
