//
// Created by Kin on 2018/9/22 0022.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINLUA_LUAVARIABLE_H
#define KINLUA_LUAVARIABLE_H

#include <string>
#include <any>

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
        template <typename T>
        LuaVariable(const T& val)
        {
            Value = val;
        }

        LuaVariable() = default;

        ~LuaVariable() = default;

        template <typename T>
        operator T()
        {
            return std::any_cast<T>(Value);
        }

    private:
        std::any Value;
    };

}
#endif //KINLUA_LUAVARIABLE_H
