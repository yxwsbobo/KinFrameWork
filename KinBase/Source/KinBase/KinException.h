//
// Created by Kin on 2018/9/23 0023.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_KINEXCEPTION_H
#define KINBASE_KINEXCEPTION_H

#include <string>
#include <exception>
#include <functional>

#include "KinCodeInfo.h"
#include "SingleObject.h"

namespace KinBase
{
    class KinException;

    namespace ExceptionInfo
    {
        class ExceptionHandlerWarpper
        {
        public:
            std::function <int(KinBase::KinException &)> Handler = [](KinBase::KinException &){return 0;};
        };
        static auto& ExceptionHandler = KinBase::SingleObject<ExceptionHandlerWarpper>::Value.Handler;
    }

    /**
     * @brief
     * @date 2018/9/23 0023
     * @author Kin
     * @copyright Copyright © 2018 jihuisoft. All rights reserved.
     */
    class KinException : public std::exception, public KinCodeInfo
    {
    public:
        using KinCodeInfo::KinCodeInfo;
        using HandlerType = std::function <int(KinBase::KinException &)>;

        explicit KinException(const KinCodeInfo& Info);

        const char *what() const noexcept override;

        static void SetExceptionHandler(const HandlerType& handler);

        static HandlerType GetExceptionHandler();

        template <typename eType>
        static int Throw(const KinBase::KinCodeInfo&) noexcept(false);

    };

    class MustException : public KinBase::KinException
    {
    public:
        using KinException::KinException;
    };

    class ShouldException : public KinBase::KinException
    {
    public:
        using KinException::KinException;
    };

#define Must(Condition, ...)                                               \
(!!(Condition))?0:KinBase::KinException::Throw<KinBase::MustException>(    \
MakeCodeInfo(#Condition,##__VA_ARGS__))

#define Should(Condition, ...)                                             \
(!!(Condition))?0:KinBase::KinException::Throw<KinBase::ShouldException>(  \
MakeCodeInfo(#Condition,##__VA_ARGS__))

}
#endif //KINBASE_KINEXCEPTION_H
