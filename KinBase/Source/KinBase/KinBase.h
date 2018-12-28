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
    auto Invoke(T&& fun, Args&& ... args)
    {
        return std::invoke(std::forward<T>(fun),std::forward<Args>(args)...);
    }

    template<typename SharedObj, typename... Args>
    auto Invoke(std::shared_ptr<SharedObj> &obj, Args &&... args)
    {
        return KinBase::Invoke(*obj, std::forward<Args>(args)...);
    }

    template <typename T,typename... Args>
    auto MakeInvoke(T&& fun,Args&&... copyArgs)
    {
        return [fun,copyArgs...](auto... args){
            return KinBase::Invoke(fun, std::forward<Args>(copyArgs)...,
                std::forward<decltype(args)...>(args...));
        };
    }


}
