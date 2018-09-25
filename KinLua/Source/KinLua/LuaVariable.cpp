#include <utility>

//
// Created by Kin on 2018/9/22 0022.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "LuaVariable.h"

#include "../Lua/lua.hpp"
#include "../Lua/KinLuaSupportInterface.h"

#include <iostream>

KinLua::LuaVariable::LuaVariable(const std::weak_ptr <KinLua::LuaCoreType> &Core,
                                 const std::string &Name, int Index)
        : Core(Core), KeyName{Name}, Index(Index)
{
    std::cout << "Create Index is :" << Index << ",Name is :" << KeyName << std::endl;
}

KinLua::LuaVariable::LuaVariable(KinLua::LuaVariable &&var) noexcept
{
    Core = std::move(var.Core);
    Index = var.Index;
    KeyName = std::move(var.KeyName);

    var.Core.reset();
    var.Index = 0;
}


KinLua::LuaVariable &KinLua::LuaVariable::operator=(KinLua::LuaVariable &&var) noexcept
{
    //Todo Delete old
    if(Index)
    {
        std::cout << "Need to Free me, Name is :" << KeyName << "Index is :" << Index;
    }

    Core = std::move(var.Core);
    Index = var.Index;
    KeyName = std::move(var.KeyName);

    var.Core.reset();
    var.Index = 0;
    return *this;
}

KinLua::LuaVariable::~LuaVariable()
{
    if(!Index)
    {
        return;
    }
    if(!Core.lock())
    {
        return;
    }
    try
    {
        auto Lua = GetCore();
        auto End = lua_gettop(Lua.get());

        Must(End != 0);

        if(End == Index)
        {
            lua_pop(Lua.get(), 1);
            std::cout << "Success Free Current :" << Index << "End :" << End << ",Name is :" << KeyName << std::endl;
        }
        else
        {
            //Todo Need to think
            std::cout << "Current :" << Index << "Last :" << End << ",Name is :" << KeyName << std::endl;
        }
    }
    catch(...)
    {
        std::cout << "LuaVariable Free exception" << std::endl;
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

KinLua::LuaVariable KinLua::LuaVariable::CreateFromIndex(int Index, const std::string &Name)
{
    auto Lua = GetCore();

    if(Index < 0)
    {
        Index = lua_absindex(Lua.get(), Index);
    }

    return KinLua::LuaVariable{Core, Name, Index};
}

KinLua::LuaValueType KinLua::LuaVariable::GetType()
{
    auto Lua = GetCore();
    if(!Index)
    {
        return GetSelfType();
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
            return CreateFromIndex(-1, Name);
        }

        default:
        {
            Must(0, "UnValid Access Use Type :", Type);
        }
    }
    return LuaVariable{Core};
}

KinLua::LuaVariable KinLua::LuaVariable::operator[](int)
{
    return KinLua::LuaVariable(Core);
}

KinLua::LuaVariable::operator bool()
{
    auto Lua = GetCore();

    return static_cast<bool>(lua_toboolean(Lua.get(), Index));
}

KinLua::LuaVariable::operator double()
{
    auto Lua = GetCore();

    return lua_tonumber(Lua.get(), Index);
}

KinLua::LuaVariable::operator long long()
{
    auto Lua = GetCore();

    return lua_tointeger(Lua.get(), Index);
}

KinLua::LuaVariable::operator std::string()
{
    auto Lua = GetCore();

    size_t Size;
    auto str = lua_tolstring(Lua.get(), Index, &Size);
    return std::string(str, Size);
}

std::vector <KinLua::LuaVariable> KinLua::LuaVariable::LuaCall(int Size)
{
    auto Lua = GetCore();

    auto Begin = lua_gettop(Lua.get()) - Size - 1;
    lua_call(Lua.get(), Size, LUA_MULTRET);
    auto End = lua_gettop(Lua.get());

    Must(End > Begin, "UnBelievable things happened!");

    std::vector <KinLua::LuaVariable> Result;

    while(End > Begin)
    {
        Result.push_back(CreateFromIndex(End--,"Return"));
    }

    return std::move(Result);

}

void KinLua::LuaVariable::MakeSpaceValid(int Size)
{
    auto Lua = GetCore();
    Should(lua_checkstack(Lua.get(), Size));
}

KinLua::LuaValueType KinLua::LuaVariable::GetSelfType()
{
    return KinLua::LuaValueType::Environment;
}

void KinLua::LuaVariable::PushSelf()
{
    auto Lua = GetCore();
    auto Type = GetType();
    //Todo
    MakeSpaceValid(1);
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

void KinLua::LuaVariable::SetValue(bool Value)
{
    auto Lua = GetCore();

}

void KinLua::LuaVariable::SetValue(double Value)
{
    auto Lua = GetCore();
}

void KinLua::LuaVariable::SetValue(long long Value)
{
    auto Lua = GetCore();
}

void KinLua::LuaVariable::SetValue(const std::string &Value)
{
    auto Lua = GetCore();

}



