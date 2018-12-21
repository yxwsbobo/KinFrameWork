#pragma once

//
// Created by Kin on 2018-10-31.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "KinException.h"
#include "KinCodeInfo.hpp"


inline KinBase::KinException::KinException(const KinBase::KinCodeInfo &Info) noexcept
    : KinCodeInfo{Info}
{ }

inline const char *KinBase::KinException::what() const noexcept
{
    return getFullInfo().c_str();
}

inline KinBase::KinException::HandlerType &KinBase::KinException::GetExceptionHandler() noexcept
{
    static HandlerType Handler;
    return Handler;
}

template<typename eType>
int KinBase::KinException::Throw(const KinBase::KinCodeInfo &Info) noexcept(false)
{
    eType ExceptionObj{Info};

    GetExceptionHandler()(ExceptionObj);

    throw ExceptionObj;
}


