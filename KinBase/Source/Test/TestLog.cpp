//
// Created by Kin on 2018-12-06.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include <catch2/catch.hpp>
#include <KinBase/KinLog.hpp>

TEST_CASE("TestLog", "normal")
{
    SECTION("KinLog with Default logger")
    {
        kInfo("Some Info use default logger");
        kInfo("Some Info use args,name :{}, age:{}","kin", 32);
    }

    SECTION("KinLog with new Logger")
    {
        auto newLogger = KinBase::KinLog::CreateLogger("newLogger");

        kInfo(newLogger,"Some Info use new logger");
        kInfo("Some Info use new logger args,name :{}, age:{}","kin", 32);
    }
}