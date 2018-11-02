//
// Created by Kin on 2018-11-02.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <KinBase/KinException.hpp>


void Throw()
{
    Must(5==3,"not equal");
}

void throw2()
{
    Should(3 == 2);
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
    throw2();
}
