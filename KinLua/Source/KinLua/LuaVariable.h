//
// Created by Kin on 2018/9/22 0022.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINLUA_LUAVARIABLE_H
#define KINLUA_LUAVARIABLE_H

#include "LuaEnumDefine.h"

#include <string>
#include <vector>
#include <memory>
#include <KinBase/KinException.h>

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
        ~LuaVariable();

        LuaValueType GetType();

        LuaVariable operator[](const std::string& Name);

        operator bool();

        inline operator int()
        {
            return (int) (long long) (*this);
        }

        inline operator unsigned int()
        {
            return (unsigned int) int(*this);
        };

        operator double();

        operator long long();

        inline operator unsigned long long()
        {
            return (unsigned long long) (long long) (*this);
        }

        operator std::string();

        template <typename T>
        inline T get()
        {
            return T(*this);
        }

        bool operator=(bool Value);

        inline int operator=(int Value)
        {
            return static_cast<int>(*this = (long long) Value);
        }

        inline unsigned int operator=(unsigned int Value)
        {
            return static_cast<unsigned int>(*this = (int) Value);
        }

        double operator=(double Value);

        long long operator=(long long Value);

        inline unsigned long long operator=(unsigned long long Value)
        {
            return static_cast<unsigned long long int>(*this = (long long) Value);
        }

        std::string operator=(const std::string &Value);


        template <typename ...ArgTypes>
        std::vector <KinLua::LuaVariable> operator()(ArgTypes && ... args)
        {
            auto Lua = GetCore();

            auto NumberOfArgs = sizeof... (args);

            MakeSpaceValid(NumberOfArgs + 1);

            auto Type = GetType();

            switch (Type)
            {
                case LuaValueType::Function :
                {
                    PushSelf();
                    (PushValue(std::forward <ArgTypes>(args)),...);
                    return LuaCall(NumberOfArgs);
                }

                default:
                {
                    Must(0, "UnValid Access Use Type :", Type);
                }
            }

            return std::vector <KinLua::LuaVariable>{};
        }


    private:
        //Todo Can't Be vector. must make sure the vector size can't change
        std::vector <KinLua::LuaVariable> LuaCall(int Size);

        void MakeSpaceValid(int Size);

        void PushSelf();

        void PushValue(bool Value);

        inline void PushValue(int Value)
        {
            return PushValue((long long) Value);
        }

        inline void PushValue(unsigned int Value)
        {
            return PushValue((int) Value);
        };

        void PushValue(double Value);

        void PushValue(long long Value);

        inline void PushValue(unsigned long long Value)
        {
            return PushValue((long long) Value);
        }

        void PushValue(const std::string &Value);


    private:
        friend class LuaEngine;

        //Todo Different Local and Global?
        LuaVariable(const std::weak_ptr <LuaCoreType> &Core, int Index = 0);

        inline std::shared_ptr <LuaCoreType> GetCore()
        {
            auto Lua = Core.lock();
            Must(Lua, "LuaCore AllReady Free");
            return Lua;
        }

        inline LuaVariable CreateFromIndex(int Index);

    private:
        std::weak_ptr <LuaCoreType> Core;
        int Index;

    };

}
#endif //KINLUA_LUAVARIABLE_H
