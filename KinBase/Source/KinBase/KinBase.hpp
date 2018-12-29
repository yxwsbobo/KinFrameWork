#pragma once

//
// Created by Kin on 2018-12-29.
// Copyright © 2018 jihuisoft. All rights reserved.
//
#include "KinBase.h"


template<typename T, typename... Args>
decltype(auto) KinBase::Invoke(T &&fun, Args &&... args)
{
    return std::invoke(std::forward<T>(fun), std::forward<Args>(args)...);
}

template<typename SharedObj, typename... Args>
decltype(auto) KinBase::Invoke(std::shared_ptr<SharedObj> &obj, Args &&... args)
{
    return KinBase::Invoke(*obj, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
decltype(auto) KinBase::MakeInvoke(T &&fun, Args &&... copyArgs)
{
    return [fun, copyArgs...](auto... args) mutable {
        return KinBase::Invoke(
            fun, std::forward<Args>(copyArgs)..., std::forward<decltype(args)>(args)...);
    };
}
