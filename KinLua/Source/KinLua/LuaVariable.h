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

#define LuaSetValueMemberFunction(Name)                                     \
void Name(bool Value);                                                      \
void Name(double Value);                                                    \
void Name(long long Value);                                                 \
void Name(const std::string& Value);                                        \
void Name(int Value){return Name((long long)Value);}                        \
void Name(unsigned int Value){return Name((long long)Value);}               \
void Name(unsigned long long Value){return Name((long long)Value);}


/**
 * @brief
 * @date 2018/9/22 0022
 * @author Kin
 * @copyright Copyright © 2018 jihuisoft. All rights reserved.
 */
    class LuaVariable
    {
    public:
        LuaVariable() = delete;

        LuaVariable(const LuaVariable &) = delete;

        LuaVariable &operator=(LuaVariable &var) = delete;

        LuaVariable(LuaVariable &&var) noexcept;

        LuaVariable &operator=(LuaVariable &&var) noexcept;
        ~LuaVariable();

        LuaValueType GetType();

        LuaVariable operator[](const std::string& Name);

        LuaVariable operator[](int tIndex);

        LuaVariable operator[](const char* Name)
        {
            return (*this)[std::string(Name)];
        }

//        LuaVariable operator[](int tIndex);

        operator bool();

        operator double();

        operator long long();

        operator std::string();

        inline operator int()
        {
            return (int) (long long) (*this);
        }

        inline operator unsigned int()
        {
            return (unsigned int) int(*this);
        };

        inline operator unsigned long long()
        {
            return (unsigned long long) (long long) (*this);
        }

        template <typename T>
        inline T get()
        {
            return T(*this);
        }

        template <typename T>
        inline const T &&operator=(const T &&Value)
        {
            MakeSpaceValid(1);
            SetValue(std::forward <T>(Value));
            return Value;
        }

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
                    return std::move(LuaCall(NumberOfArgs));
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

        KinLua::LuaValueType GetSelfType();

        LuaSetValueMemberFunction(PushValue)

        LuaSetValueMemberFunction(SetValue)


    private:
        friend class LuaEngine;

        //Todo Different Local and Global?
        LuaVariable(const std::weak_ptr <LuaCoreType> &Core,
                    const std::string &Name = "Core", int Index = 0);

        inline std::shared_ptr <LuaCoreType> GetCore()
        {
            auto Lua = Core.lock();
            Must(Lua, "LuaCore AllReady Free");
            return Lua;
        }

        inline LuaVariable CreateFromIndex(int Index, const std::string &Name);

    private:
        std::weak_ptr <LuaCoreType> Core;
        // std::vector<std::shared_ptr<KinLua::LookupInfo>> ElementLists;
        int Index;
        std::string KeyName;
        
    };

}
#endif //KINLUA_LUAVARIABLE_H
