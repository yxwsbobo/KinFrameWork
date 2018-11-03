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
{ }

inline const char *KinBase::KinException::what() const noexcept
{
    return getFullInfo().c_str();
}

inline KinBase::KinException::HandlerListsType &KinBase::KinException::GetExceptionHandlerLists()
{
    static HandlerListsType Lists{};
    return Lists;
}

inline void KinBase::KinException::AddExceptionHandler(
    const std::string_view &tag,
    const KinBase::KinException::HandlerType &handler
)
{
    GetExceptionHandlerLists()[tag] = handler;
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
