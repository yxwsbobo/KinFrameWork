//
// Created by Kin on 2018-10-31.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_KINCODEINFO_HPP
#define KINBASE_KINCODEINFO_HPP

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

inline const std::string &KinBase::KinCodeInfo::getFullInfo() const
{
    FullInfo = fmt::format(
        "{}():{}({}) [{}]: {}", getFunctionName(), getFileName(), getLineNumber(), getCondition(), getMessage());

    return FullInfo;
}

inline const std::string &KinBase::KinCodeInfo::getMessage() const
{
    return Message;
}

inline void KinBase::KinCodeInfo::setMessage(const std::string &Message)
{
    KinCodeInfo::Message = Message;
}

inline const std::string_view &KinBase::KinCodeInfo::getCondition() const
{
    return Condition;
}

inline void KinBase::KinCodeInfo::setCondition(const std::string_view &Condition)
{
    KinCodeInfo::Condition = Condition;
}

inline const std::string_view KinBase::KinCodeInfo::getFileName() const
{
    return getFullFileName().substr(getFullFileName().find_last_of(R"(\/)") + 1);
}

inline const std::string_view &KinBase::KinCodeInfo::getFullFileName() const
{
    return FileName;
}

inline void KinBase::KinCodeInfo::setFileName(const std::string_view &FileName)
{
    KinCodeInfo::FileName = FileName;
}

inline const std::string_view KinBase::KinCodeInfo::getFunctionName() const
{
    return GetFullFunctionName().substr(GetFullFunctionName().find_last_of(R"(:)") + 1);
}

inline const std::string_view &KinBase::KinCodeInfo::GetFullFunctionName() const
{
    return FunctionName;
}

inline void KinBase::KinCodeInfo::setFunctionName(const std::string_view &FunctionName)
{
    KinCodeInfo::FunctionName = FunctionName;
}

inline int KinBase::KinCodeInfo::getLineNumber() const
{
    return LineNumber;
}

inline void KinBase::KinCodeInfo::setLineNumber(int LineNumber)
{
    KinCodeInfo::LineNumber = LineNumber;
}

template<typename T1, typename T2, typename T3, typename T4, typename... ArgTypes>
KinBase::KinCodeInfo KinBase::KinCodeInfo::Create(
    T1 &&Condition, T2 &&FileName, T3 &&FunctionName, T4 &&LineNumber, ArgTypes &&... args
) noexcept(false)
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
) noexcept(false)
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


#endif //KINBASE_KINCODEINFO_HPP
