//
// Created by Kin on 2018-12-06.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include <catch2/catch.hpp>
#include <KinBase/KinException.hpp>
#include <KinBase/KinLog.hpp>
#include <thread>
#include <chrono>

TEST_CASE("TestException", "normal")
{
    SECTION("Must Fail Test")
    {
        int Changed = 0;
        try
        {
            Must(1 == 2);
        }
        catch (KinBase::KinException& e)
        {
            kInfo("Catch Exception :{}",e);
            Changed = 1;
        }
        REQUIRE(Changed == 1);
    }

    SECTION("Must Fail Test With Info")
    {
        int Changed = 0;
        try
        {
            Must(false,"info on fail");
        }
        catch (KinBase::KinException& e)
        {
            kInfo("Catch Exception :{}",e);
            Changed = 1;
        }
        REQUIRE(Changed == 1);
    }

    SECTION("Should Fail Test")
    {
        int Changed = 0;
        try
        {
            Should(1 == 2);
        }
        catch (KinBase::KinException& e)
        {
            kInfo("Catch Exception :{}",e);
            Changed = 1;
        }
        REQUIRE(Changed == 1);
    }

    SECTION("Should Fail Test With Info")
    {
        int Changed = 0;
        try
        {
            Should(false,"info on fail");
        }
        catch (KinBase::KinException& e)
        {
            kInfo("Catch Exception :{}",e);
            Changed = 1;
        }
        REQUIRE(Changed == 1);
    }

    SECTION("Must Test")
    {
        Must(true);
    }

    SECTION("Must Test With Info")
    {
        Must(true,"info on fail");
    }

    SECTION("Should Test")
    {
        Should(true);
    }

    SECTION("Should Test With Info")
    {
        Should(true,"info on fail");
    }

    SECTION("Test Exception Handle")
    {
        int Handled = 0;
        int catches = 0;
        KinBase::KinException::GetExceptionHandler().connect([&Handled](auto& e){
            kInfo("Handle Exception :{}",e);
            Handled = 1;
            return 1;
        });

        try
        {
            Must(false,"Need be handled");
        }
        catch (KinBase::KinException& e)
        {
            catches = 1;
        }

        REQUIRE(Handled);
        REQUIRE(catches);
    }
}
