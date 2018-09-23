//
// Created by Kin on 2018/9/23 0023.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "KinException.h"

std::function <int(KinBase::KinException &)> KinBase::ExceptionHandler = nullptr;

KinBase::KinException::KinException(
        const std::string &Message, const std::string &Condition,
        const std::string &FileName, const std::string &FunctionName,
        const std::string &LineNumber)
        : Message(Message), Condition(Condition), FileName(FileName),
        FunctionName(FunctionName), LineNumber(LineNumber)
{
}

const char *KinBase::KinException::what() const noexcept
{
    return Message.c_str();
}

const std::string &KinBase::KinException::GetCondition() const noexcept
{
    return Condition;
}

const std::string &KinBase::KinException::GetFileName() const noexcept
{
    return FileName;
}

const std::string &KinBase::KinException::GetFunctionName() const noexcept
{
    return FunctionName;
}

const std::string &KinBase::KinException::GetLineNumber() const noexcept
{
    return LineNumber;
}

void KinBase::KinException::SetMessage(const std::string &Message) noexcept
{
    KinException::Message = Message;
}

void KinBase::KinException::SetCondition(const std::string &Condition) noexcept
{
    KinException::Condition = Condition;
}

void KinBase::KinException::SetFileName(const std::string &FileName) noexcept
{
    KinException::FileName = FileName;
}

void KinBase::KinException::SetFunctionName(const std::string &FunctionName) noexcept
{
    KinException::FunctionName = FunctionName;
}

void KinBase::KinException::SetLineNumber(const std::string &LineNumber) noexcept
{
    KinException::LineNumber = LineNumber;
}
