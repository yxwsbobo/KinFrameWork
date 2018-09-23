//
// Created by Kin on 2018/9/22 0022.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINLUA_LUAENGINE_H
#define KINLUA_LUAENGINE_H

#include "LuaEnumDefine.h"
#include "LuaVariable.h"
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
    class LuaEngine
    {
    public:
    public:
        LuaEngine();

        ~LuaEngine();

        /**
         * @brief Load lua script and run
         * @param FilePath
         * @return Original lua value
         */
        int Load(const std::string &FilePath);

        /**
         * @brief Load Lua code and run
         * @param Code
         * @return Original Lua value
         */
        int LoadCode(const std::string &Code);

        LuaVariable operator[](const std::string& Name);

        /**
         * @brief Call Lua Function
         * @tparam ArgTypes
         * @param FunName
         * @param args
         */
        template <typename ...ArgTypes>
        void Call(const std::string &FunName, ArgTypes &&... args)
        {
            std::vector <LuaVariable> Result;
            CallR(FunName, Result, std::forward <ArgTypes>(args)...);
        }

        /**
         * @brief Call Lua Function With Result
         * @tparam ArgTypes
         * @param FunName
         * @param Results the Results.size() decide how many values need return
         * @param args
         */
        template <typename ...ArgTypes>
        void CallR(const std::string &FunName, std::vector <LuaVariable> &Results,
                   ArgTypes &&... args)
        {
            auto ArgNumbers = sizeof...(args);

            LuaPushGlobalVariable(FunName);
            PushArgs(std::forward <ArgTypes>(args)...);
            LuaCall(ArgNumbers, Results);
        }

        /**
         * @brief Return one value
         * @tparam ArgTypes
         * @param FunName
         * @param args
         * @return
         */
        template <typename ...ArgTypes>
        LuaVariable CallR(const std::string &FunName, ArgTypes &&... args)
        {
            std::vector <LuaVariable> Result{1};
            CallR(FunName, Result, std::forward <ArgTypes>(args)...);
            return Result[0];
        }


    private:
        template <typename ...ArgTypes>
        void PushArgs(ArgTypes &&... args)
        {
            (LuaPushArg(std::forward <ArgTypes>(args)),...);
        }

    private:
        void LuaPushGlobalVariable(const std::string &Name);

        void LuaCall(int ArgNums, std::vector <LuaVariable> &ReturnValue);

        void LuaPushArg(const std::string &Arg);

        void LuaPushArg(int Number);

        void LuaPushArg(double Number);


    private:
        void* Core;

    };

}
#endif //KINLUA_LUAENGINE_H
