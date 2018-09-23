//
// Created by Kin on 2018/9/22 0022.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINLUA_LUAVARIABLE_H
#define KINLUA_LUAVARIABLE_H

#include "LuaEnumDefine.h"

#include <string>
#include <vector>

namespace KinLua
{

/**
 * @brief
 * @date 2018/9/22 0022
 * @author Kin
 * @copyright Copyright © 2018 jihuisoft. All rights reserved.
 */
    class LuaVariable
    {
    public:

        LuaVariable(void *Core, int Index, LuaValueType Type);

        ~LuaVariable();

        LuaVariable operator[](const std::string& Name);

        template <typename ...ArgTypes>
        std::vector <KinLua::LuaVariable> operator()(ArgTypes && ... args)
        {

        }

        template <typename T>
        operator T()
        {
//            return std::any_cast<T>(Value);
        }

    private:
        void* Core;
        int Index;
        LuaValueType Type;

    };

}
#endif //KINLUA_LUAVARIABLE_H
