//
// Created by Kin on 2018-11-02.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <KinBase/KinException.hpp>
#include <thread>
#include <chrono>

void Throw()
{
    Must(5!=3);
}

void throw2()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    Should(3 != 2);
}

TEST_CASE("Test1", "[single-file]")
{
    SECTION("Run Throw")
    {
        Throw();

    }

    SECTION("Run Throw2")
    {
        throw2();
    }
}

TEST_CASE("Maybe name", "[single-file]")
{
    //    REQUIRE( Factorial(0) == 1 );
    SECTION("also run throw2")
    {

        throw2();
    }

    SECTION("tis is success")
    {
        Must(5 == 5);
    }
}

TEST_CASE("Kin Test", "[Kin Tag]")
{

    SECTION("ttis is success")
    {
        Must(5 == 5);
    }
}