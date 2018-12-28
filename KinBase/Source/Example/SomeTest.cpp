//
// Created by Kin on 2018/12/22.
// Copyright © 2018 jihuisoft. All rights reserved.
//
#include <KinBase/KinBase.h>
#include <memory>
#include <iostream>
class MyClas
{
public:
    MyClas() = default;

    int operator()(int,double)
    {
        return 5;
    }

    int Run(int,double)
    {
        return 7;
    }
};

int main()
{
    auto mc = std::make_shared<MyClas>();

    MyClas mmc;

    int Result = KinBase::Invoke(mc,3,2.0);

    std::cout<<Result<<std::endl;

    return 0;
}