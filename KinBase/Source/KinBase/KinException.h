//
// Created by Kin on 2018/9/23 0023.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_KINEXCEPTION_H
#define KINBASE_KINEXCEPTION_H

#include <string>
#include <exception>
#include <functional>
#include <fmt/fmt.h>

#include "KinMacroDefine.h"

namespace KinBase
{
    class KinException;

    extern std::function <int(KinBase::KinException &)> ExceptionHandler;

/**
 * @brief
 * @date 2018/9/23 0023
 * @author Kin
 * @copyright Copyright © 2018 jihuisoft. All rights reserved.
 */
    class KinException : public std::exception
    {
    public:
        KinException(const std::string &Message, const std::string &Condition, const std::string &FileName,
                     const std::string &FunctionName, const std::string &LineNumber);

        ~KinException() override = default;

        const char *what() const noexcept override;

        const std::string &GetCondition() const noexcept;

        const std::string &GetFileName() const noexcept;

        const std::string &GetFunctionName() const noexcept;

        const std::string &GetLineNumber() const noexcept;

        void SetMessage(const std::string &Message) noexcept;

        void SetCondition(const std::string &Condition) noexcept;

        void SetFileName(const std::string &FileName) noexcept;

        void SetFunctionName(const std::string &FunctionName) noexcept;

        void SetLineNumber(const std::string &LineNumber) noexcept;

        template <typename T, typename T1, typename T2, typename T3, typename T4, typename... ArgTypes>
        static int
        Throw(T1 &&Condition, T2 &&FileName, T3 &&FunctionName, T4 &&LineNumber, ArgTypes &&... args) noexcept(false)
        {
            T ExceptionObj{
                    fmt::format(std::forward <ArgTypes>(args)...),
                    fmt::format("{}", Condition),
                    fmt::format("{}", FileName),
                    fmt::format("{}", FunctionName),
                    fmt::format("{}", LineNumber)};

            if(ExceptionHandler)
            {
                try
                {
                    auto Result = ExceptionHandler(ExceptionObj);
                    if(Result != 0)
                    {
                        return Result;
                    }
                }
                catch(...)
                {

                }
            }

            throw std::move(ExceptionObj);

        }

        template <typename T, typename T1, typename T2, typename T3, typename T4>
        static inline int Throw(T1 &&Condition, T2 &&FileName, T3 &&FunctionName, T4 &&LineNumber) noexcept(false)
        {
            return Throw <T>(
                    std::forward <T1>(Condition),
                    std::forward <T2>(FileName),
                    std::forward <T3>(FunctionName),
                    std::forward <T4>(LineNumber),
                    fmt::format("{}", Condition));
        }

    protected:
        std::string Message;
        std::string Condition;
        std::string FileName;
        std::string FunctionName;
        std::string LineNumber;

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


#define Must(Condition, ...)                                             \
(void)(                                                                  \
(!!(Condition)) ||                                                       \
        (KinBase::KinException::Throw<KinBase::MustException>(           \
        #Condition,                                                      \
        MACRO_GET_FILE_NAME(),                                           \
        __FUNCTION__,                                                    \
        __LINE__,                                                        \
        ##__VA_ARGS__                                                    \
        ), 0)                                                            \
)

#define Should(Condition, ...)                                           \
(void)(                                                                  \
(!!(Condition)) ||                                                       \
        (KinBase::KinException::Throw<KinBase::ShouldException>(         \
        #Condition,                                                      \
        MACRO_GET_FILE_NAME(),                                           \
        __FUNCTION__,                                                    \
        __LINE__,                                                        \
        ##__VA_ARGS__                                                    \
        ), 0)                                                            \
)

}
#endif //KINBASE_KINEXCEPTION_H
