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


    auto List = Engine["ComputeAdd"](5, 10);

    std::cout << "start output" << std::endl;

    for(auto &item : List)
    {
        int a = item;

        std::cout<<"Receive :"<<a<<std::endl;
    }

    std::string Name = Engine["Name"];
    std::cout<<Name<<" is "<< Engine["Age"].get<int>()<<std::endl;


    return 0;
}