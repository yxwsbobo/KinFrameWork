//
// Created by Kin on 2018/9/22 0022.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINLUA_LUAENGINE_H
#define KINLUA_LUAENGINE_H

#include "LuaEnumDefine.h"
#include "LuaVariable.h"
#include <string>
#include <memory>

namespace KinLua
{

/**
 * @brief
 * @date 2018/9/22 0022
 * @author Kin
 * @copyright Copyright © 2018 jihuisoft. All rights reserved.
 */
    class LuaEngine
    {
    public:
        LuaEngine();

        ~LuaEngine() = default;

        /**
         * @brief Load lua script and run
         * @param FilePath
         * @return Original lua value
         */
        int Run(const std::string &FilePath);

        /**
         * @brief Load Lua code and run
         * @param Code
         * @return Original Lua value
         */
        int RunCode(const std::string &Code);

        inline LuaVariable operator[](const std::string &Name)
        {
            return LuaVariable{Core}[Name];
        }

    private:
        std::shared_ptr <LuaCoreType> Core;

    };

}
#endif //KINLUA_LUAENGINE_H
