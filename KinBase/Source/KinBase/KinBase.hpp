#pragma once

//
// Created by Kin on 2018-12-29.
// Copyright © 2018 jihuisoft. All rights reserved.
//
#include "KinBase.h"
#include "KinException.hpp"
#include <type_traits>


template<typename T, typename... Args>
decltype(auto) KinBase::Invoke(T &&fun, Args &&... args)
{
    if constexpr (std::is_convertible_v<T,std::shared_ptr<void>>)
    {
        return std::invoke(*fun, std::forward<Args>(args)...);
    }
    else if constexpr (std::is_convertible_v<T,std::weak_ptr<void>>)
    {
        auto fn = fun.lock();
        Must(fn,"Invoke a not exist week_ptr");
        return std::invoke(*fn, std::forward<Args>(args)...);
    }
    else
    {
        return std::invoke(std::forward<T>(fun), std::forward<Args>(args)...);
    }
}

template<typename T, typename... Args>
decltype(auto) KinBase::MakeInvoke(T &&fun, Args &&... copyArgs)
{
    return [fun, copyArgs...](auto... args) mutable {
        return KinBase::Invoke(
            fun, std::forward<Args>(copyArgs)..., std::forward<decltype(args)>(args)...);
    };
}
