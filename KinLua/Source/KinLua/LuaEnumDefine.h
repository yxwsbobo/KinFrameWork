//
// Created by Kin on 2018/9/22 0022.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINLUA_LUAENUMDEFINE_H
#define KINLUA_LUAENUMDEFINE_H

#include <KinBase/BetterEnum.h>
#include <string>
struct lua_State;

namespace KinLua
{
    using LuaCoreType = ::lua_State;

    BETTER_ENUM(LuaValueType, int,
                None = -1,
                Nil = 0,
                Boolean = 1,
                LightUserData = 2,
                Number = 3,
                String = 4,
                Table = 5,
                Function = 6,
                UserData = 7,
                Thread = 8,
                LuaTypeOver = 9,
                Environment = 0x80
    );

//    BETTER_ENUM(ElementType, int, Environment, Index, String, Stack);
//
//    struct LookupInfo
//    {
//        int Type;
//        union
//        {
//            std::string Name;
//            int Index;
//        } Value;
//    };
}

#endif //KINLUA_LUAENUMDEFINE_H
