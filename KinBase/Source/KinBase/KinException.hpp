//
// Created by Kin on 2018-10-31.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_KINEXCEPTION_HPP
#define KINBASE_KINEXCEPTION_HPP

#include "KinException.h"
#include "KinCodeInfo.hpp"


inline KinBase::KinException::KinException(const KinBase::KinCodeInfo &Info)
    : KinCodeInfo{Info}
{
}

inline const char *KinBase::KinException::what() const noexcept
{
    return getFullInfo().c_str();
}

inline KinBase::KinException::HandlerType &KinBase::KinException::GetExceptionHandler()
{
    static HandlerType Handler = [](KinBase::KinException &) { return 0; };
    return Handler;
}

inline void KinBase::KinException::SetExceptionHandler(const KinBase::KinException::HandlerType &handler)
{
    GetExceptionHandler() = handler;
}

template<typename eType>
int KinBase::KinException::Throw(const KinBase::KinCodeInfo &Info) noexcept(false)
{
    eType ExceptionObj{Info};

    if (GetExceptionHandler())
    {
        try
        {
            auto Result = GetExceptionHandler()(ExceptionObj);
            if (Result != 0)
            {
                return Result;
            }
        }
        catch (...)
        {

        }
    }

    throw ExceptionObj;
}

#endif //KINBASE_KINEXCEPTION_HPP
