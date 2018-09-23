#include <utility>

//
// Created by Kin on 2018/9/22 0022.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "LuaVariable.h"

KinLua::LuaVariable::LuaVariable(void *Core, int Index, KinLua::LuaValueType Type)
: Core(Core), Index(Index), Type(Type)
{
}

KinLua::LuaVariable::~LuaVariable()
{
    Core = nullptr;
}

KinLua::LuaVariable KinLua::LuaVariable::operator[](const std::string& Name)
{
    return LuaVariable{Core,1,LuaValueType::Integer};
}
