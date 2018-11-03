//
// Created by Kin on 2018-11-01.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "MyTestLib.h"
#include <KinBase/KinException.hpp>
#include <KinBase/KinLog.hpp>
#include <iostream>

void KinBase::MyTestLib::LibRun()
{
    kInfo("Start to Log");
    kWarn("number is {}", 7);
    kError();

    auto lg = KinBase::KinLog::CreateLogger("lb");

    kInfo(lg,"Start Dest Log");
    kWarn(lg,"number is {}", 27);
    kError(lg);

    try
    {
        Must(7 != 2, "must error");
        Should(7 != 2);
        Must(7 == 3,"must error");
        Should(7 == 2,"should error not equal");
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

KinBase::MyTestLib::MyTestLib()
{
    auto Handler = KinBase::KinException::GetExceptionHandler();

    KinBase::KinException::SetExceptionHandler([Handler](KinBase::KinException& e){
        kInfo("MyLib Exception :{}", e.getCondition());
        int result =0;
        if(e.getLineNumber() == 27)
        {
            result = Handler(e);
            return 1;
        }
        return result;
    });
}
