//
// Created by Kin on 2018/9/23 0023.
// Copyright © 2018 jihuisoft. All rights reserved.
//


#include <iostream>

#include <KinBase/KinException.hpp>
#include <KinBase/KinLog.hpp>
#include "TestClass.h"
#include "MyTestLib.h"

namespace NSpace
{
    class MyTestClass
    {
    public:
        void RunSomeCode()
        {
            Must(5 != 0, "must error");
            Should(3 != 0);
            Must(5 == 3,"must error");
            Should(5 == 0,"should error not equal");
        }
    };
}


int main()
{
    std::cout<<"start"<<std::endl;

    kInfo("Start to Log");
    kWarn("number is {}", 5);
    kError();

    auto lg = KinBase::KinLog::CreateLogger("ac");

    kInfo(lg,"Start Dest Log");
    kWarn(lg,"number is {}", 15);
    kError(lg);

    NSpace::MyTestClass mt;
    KinBase::KinException::SetExceptionHandler([](KinBase::KinException& e){
        kInfo("Handle Exception :{}", e.getCondition());

        if(std::stoi(e.getLineNumber()) == 23)
        {
            return 1;
        }

        e.setMessage(fmt::format("have handled the exception. origin is: {}", e.getMessage()));
        return 0;
    });

    try
    {
        mt.RunSomeCode();
    }
    catch(const KinBase::KinException &e)
    {
        std::cout << e.what() << std::endl;
        std::cout << e.getCondition() << std::endl;
        std::cout << e.getFileName() << std::endl;
        std::cout << e.getLineNumber() << std::endl;
        std::cout << e.getFunctionName() << std::endl;
    }

    KinBase::MyTestLib myTestLib;

    std::cout<<std::endl<<std::endl;

    KinBase::TestClass tc;

    tc.Run();

    myTestLib.LibRun();

    return 0;
}