//
// Created by Kin on 2018-11-01.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "TestClass.h"
#include <KinBase/KinException.hpp>
#include <KinBase/KinLog.hpp>
#include <iostream>


void KinBase::TestClass::Run()
{
    kInfo("Start to Log");
    kWarn("number is {}", 5);
    kError();

    auto lg = KinBase::KinLog::CreateLogger("at");

    kInfo(lg,"Start Dest Log");
    kWarn(lg,"number is {}", 15);
    kError(lg);

    try
    {
        Must(5 != 2, "must error");
        Should(3 != 2);
        Must(5 == 3,"must error");
        Should(5 == 2,"should error not equal");
    }
    catch(const KinBase::KinException &e)
    {
        std::cout << e.what() << std::endl;
        std::cout << e.getCondition() << std::endl;
        std::cout << e.getFileName() << std::endl;
        std::cout << e.getLineNumber() << std::endl;
        std::cout << e.getFunctionName() << std::endl;
    }

}
