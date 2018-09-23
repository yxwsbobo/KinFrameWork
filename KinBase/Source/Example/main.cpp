//
// Created by Kin on 2018/9/23 0023.
// Copyright © 2018 jihuisoft. All rights reserved.
//


#include <iostream>

#include <KinBase/KinException.h>

namespace NSpace
{
    class MyTestClass
    {
    public:
        void RunSomeCode()
        {
            Must(5 == 0, "must error");

            Should(5 == 0, "should error");

            Must(5 != 3);
            Should(5 != 0);
        }
    };
}


int main()
{
    NSpace::MyTestClass mt;

    try
    {
        mt.RunSomeCode();
    }
    catch(const KinBase::KinException &e)
    {
        std::cout << e.what() << std::endl;
        std::cout << e.GetCondition() << std::endl;
        std::cout << e.GetFileName() << std::endl;
        std::cout << e.GetLineNumber() << std::endl;
        std::cout << e.GetFunctionName() << std::endl;
    }


    return 0;
}