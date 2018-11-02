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
    const std::string &Condition,
    const std::string &FileName,
    const std::string &FunctionName,
    const std::string &LineNumber
)
    : Message(Message), Condition(Condition), FileName(FileName), FunctionName(FunctionName), LineNumber(LineNumber)
{
    FullInfo = fmt::format("{}(){}:{} [{}]: {}",FunctionName,FileName,LineNumber,Condition,Message);
}

inline const std::string &KinBase::KinCodeInfo::getMessage() const
{
    return Message;
}

inline void KinBase::KinCodeInfo::setMessage(const std::string &Message)
{
    KinCodeInfo::Message = Message;
}

inline const std::string &KinBase::KinCodeInfo::getCondition() const
{
    return Condition;
}

inline void KinBase::KinCodeInfo::setCondition(const std::string &Condition)
{
    KinCodeInfo::Condition = Condition;
}

inline const std::string &KinBase::KinCodeInfo::getFileName() const
{
    return FileName;
}

inline void KinBase::KinCodeInfo::setFileName(const std::string &FileName)
{
    KinCodeInfo::FileName = FileName;
}

inline const std::string &KinBase::KinCodeInfo::getFunctionName() const
{
    return FunctionName;
}

inline void KinBase::KinCodeInfo::setFunctionName(const std::string &FunctionName)
{
    KinCodeInfo::FunctionName = FunctionName;
}

inline const std::string &KinBase::KinCodeInfo::getLineNumber() const
{
    return LineNumber;
}

inline void KinBase::KinCodeInfo::setLineNumber(const std::string &LineNumber)
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
        fmt::format("{}", std::forward<T1>(Condition)),
        fmt::format("{}", std::forward<T2>(FileName)),
        fmt::format("{}", std::forward<T3>(FunctionName)),
        fmt::format("{}", std::forward<T4>(LineNumber)));
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

const std::string &KinBase::KinCodeInfo::getFullInfo() const
{
    return FullInfo;
}

inline std::ostream &operator<<(std::ostream &os, const KinBase::KinCodeInfo &CodeInfo)
{
    os << CodeInfo.getFullInfo();
    return os;
}


#endif //KINBASE_KINCODEINFO_HPP
