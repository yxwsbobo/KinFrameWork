//
// Created by Kin on 2018/9/24 0024.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINLUA_KINLUASUPPORTINTERFACE_H
#define KINLUA_KINLUASUPPORTINTERFACE_H

#include "lua.hpp"
namespace KinLua
{
    namespace Interface
    {
        template <typename T>
        void SetValue(T&& Value)
        {
//            lua_pop;
        }
    }

/**
 * @brief
 * @date 2018/9/24 0024
 * @author Kin
 * @copyright Copyright © 2018 jihuisoft. All rights reserved.
 */
    class KinLuaSupportInterface
    {
    public:
        KinLuaSupportInterface() = default;

        ~KinLuaSupportInterface() = default;


    };

}
#endif //KINLUA_KINLUASUPPORTINTERFACE_H
