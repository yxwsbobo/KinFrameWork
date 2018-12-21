#pragma once

//
// Created by Kin on 2018-10-31.
// Copyright © 2018 jihuisoft. All rights reserved.
//


#include "KinCodeInfo.h"

#include <fmt/fmt.h>
#include <fmt/ostr.h>


inline KinBase::KinCodeInfo::KinCodeInfo(
    const std::string &Message,
    const std::string_view &Condition,
    const std::string_view &FileName,
    const std::string_view &FunctionName,
    int LineNumber
)
    : Message(Message), Condition(Condition), FileName(FileName), FunctionName(FunctionName), LineNumber(LineNumber)
{ }

inline const std::string &KinBase::KinCodeInfo::getFullInfo() const noexcept
{
    FullInfo = fmt::format(
        "{}():{}({}) [{}]: {}", getFunctionName(), getFileName(), getLineNumber(), getCondition(), getMessage());

    return FullInfo;
}

inline const std::string &KinBase::KinCodeInfo::getMessage() const noexcept
{
    return Message;
}

inline void KinBase::KinCodeInfo::setMessage(const std::string &Message) noexcept
{
    KinCodeInfo::Message = Message;
}

inline const std::string_view &KinBase::KinCodeInfo::getCondition() const noexcept
{
    return Condition;
}

inline void KinBase::KinCodeInfo::setCondition(const std::string_view &Condition) noexcept
{
    KinCodeInfo::Condition = Condition;
}

inline const std::string_view KinBase::KinCodeInfo::getFileName() const noexcept
{
    return getFullFileName().substr(getFullFileName().find_last_of(R"(\/)") + 1);
}

inline const std::string_view &KinBase::KinCodeInfo::getFullFileName() const noexcept
{
    return FileName;
}

inline void KinBase::KinCodeInfo::setFileName(const std::string_view &FileName) noexcept
{
    KinCodeInfo::FileName = FileName;
}

inline const std::string_view KinBase::KinCodeInfo::getFunctionName() const noexcept
{
    return GetFullFunctionName().substr(GetFullFunctionName().find_last_of(R"(:)") + 1);
}

inline const std::string_view &KinBase::KinCodeInfo::GetFullFunctionName() const noexcept
{
    return FunctionName;
}

inline void KinBase::KinCodeInfo::setFunctionName(const std::string_view &FunctionName) noexcept
{
    KinCodeInfo::FunctionName = FunctionName;
}

inline int KinBase::KinCodeInfo::getLineNumber() const noexcept
{
    return LineNumber;
}

inline void KinBase::KinCodeInfo::setLineNumber(int LineNumber) noexcept
{
    KinCodeInfo::LineNumber = LineNumber;
}

template<typename T1, typename T2, typename T3, typename T4, typename... ArgTypes>
KinBase::KinCodeInfo KinBase::KinCodeInfo::Create(
    T1 &&Condition, T2 &&FileName, T3 &&FunctionName, T4 &&LineNumber, ArgTypes &&... args
) noexcept
{
    return KinBase::KinCodeInfo(
        fmt::format(std::forward<ArgTypes>(args)...),
        std::forward<T1>(Condition),
        std::forward<T2>(FileName),
        std::forward<T3>(FunctionName),
        std::forward<T4>(LineNumber));
}

template<typename T1, typename T2, typename T3, typename T4>
KinBase::KinCodeInfo KinBase::KinCodeInfo::Create(
    T1 &&Condition, T2 &&FileName, T3 &&FunctionName, T4 &&LineNumber
) noexcept
{
    return Create(
        std::forward<T1>(Condition),
        std::forward<T2>(FileName),
        std::forward<T3>(FunctionName),
        std::forward<T4>(LineNumber),
        fmt::format("Condition Fail :{}", Condition));
}

inline std::ostream &operator<<(std::ostream &os, const KinBase::KinCodeInfo &CodeInfo)
{
    os << CodeInfo.getFullInfo();
    return os;
}

