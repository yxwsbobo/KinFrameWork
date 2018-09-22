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

    Engine.Load("../../../Examples/testLua.lua");
    Engine.LoadCode("ptFibona(10)");

    std::cout<<"Start To Call ---------"<<std::endl;
    
    double a = Engine.CallR("callTest",3,5);

    std::cout<<"Result is :"<<a<<std::endl;

    return 0;
}