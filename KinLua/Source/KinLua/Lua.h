//
// Created by Kin on 2018/9/22 0022.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINLUA_LUA_H
#define KINLUA_LUA_H

#include <string>
#include <memory>
#include "LuaEnumDefine.h"

struct lua_State;

namespace KinLua
{
    using LuaCoreType = lua_State;

/**
 * @brief
 * @date 2018/9/22 0022
 * @author Kin
 * @copyright Copyright © 2018 jihuisoft. All rights reserved.
 */
    class Lua
    {
    public:
        Lua();
        virtual ~Lua();

        ResultEnum RunFile(std::string Path);
        ResultEnum RunCode(std::string Code);


    private:
        std::shared_ptr<LuaCoreType> Core;
    };

}
#endif //KINLUA_LUA_H
