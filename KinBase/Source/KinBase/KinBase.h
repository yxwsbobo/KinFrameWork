#pragma once

//
// Created by Kin on 2018/12/23.
// Copyright © 2018 jihuisoft. All rights reserved.
//
#include <functional>
#include <memory>

template <typename... T>
void Run(T&&... args)
{

}

namespace KinBase
{
    template <typename T>
    class TT;

    template <typename T, typename... Args>
    decltype(auto) Invoke(T&& fun, Args&& ... args);

    template<typename SharedObj, typename... Args>
    decltype(auto) Invoke(std::shared_ptr<SharedObj> &obj, Args &&... args);

    template <typename T,typename... Args>
    decltype(auto) MakeInvoke(T&& fun,Args&&... copyArgs);

}
