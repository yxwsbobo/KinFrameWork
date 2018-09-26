//
// Created by Kin on 2018/9/22 0022.
// Copyright © 2018 jihuisoft. All rights reserved.
//


#include <string>
#include <iostream>
#include <KinLua/LuaEngine.h>

int main()
{
    KinLua::LuaEngine Engine;

    Engine.Run("../../../Examples/testLua.lua");

//    auto People = Engine["People"];
//
//    People["Age"] = 26;
//
//    auto School = People["school"];
//
//    int Numbers = School["MakeParty"]("周一",50)[0];
//
//    std::cout<<Numbers <<" 个人参加 "<<std::endl;

    return 0;
}