#pragma once

//
// Created by Kin on 2018-10-31.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "KinLog.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <fmt/fmt.h>
#include <fmt/ostr.h>


inline KinBase::KinLog::KinDefaultLoggerWarpper::KinDefaultLoggerWarpper() noexcept
{
    Logger = spdlog::stdout_color_mt("log");
    spdlog::set_pattern("%Y-%m-%d %T|%t|%n [%L] %v");
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::trace);

    Logger->set_pattern("%^%Y-%m-%d %T|%t|%n [%L] %v%$");
}

inline std::shared_ptr<spdlog::logger> &KinBase::KinLog::GetDefaultLogger() noexcept
{
    static KinDefaultLoggerWarpper DefaultLogger;
    return DefaultLogger.Logger;
}

inline void KinBase::KinLog::SetDefaultLogger(const std::shared_ptr<spdlog::logger> &Logger) noexcept
{
    GetDefaultLogger() = Logger;
}

template<typename... ArgTypes>
std::shared_ptr<spdlog::logger> KinBase::KinLog::CreateLogger(ArgTypes &&... args)
{
    return spdlog::stdout_color_mt(std::forward<ArgTypes>(args)...);
}

template<typename T1, typename T2, typename T3, typename T4, typename... ArgTypes>
void KinBase::KinLog::Log(T1 &&Level, T2 &&FileName, T3 &&FunctionName, T4 &&LineNumber, ArgTypes &&... args) noexcept
{
    return Log(
        std::forward<T1>(Level),
        std::forward<T2>(FileName),
        std::forward<T3>(FunctionName),
        std::forward<T4>(LineNumber),
        GetDefaultLogger(),
        std::forward<ArgTypes>(args)...
    );
}

template<typename T1, typename T2, typename T3, typename T4>
void KinBase::KinLog::Log(T1 &&Level, T2 &&FileName, T3 &&FunctionName, T4 &&LineNumber) noexcept
{
    return Log(
        std::forward<T1>(Level),
        std::forward<T2>(FileName),
        std::forward<T3>(FunctionName),
        std::forward<T4>(LineNumber),
        ""
    );
}

template<typename T1, typename T2, typename T3, typename T4, typename... ArgTypes>
void KinBase::KinLog::Log(
    T1 &&Level,
    T2 &&FileName,
    T3 &&FunctionName,
    T4 &&LineNumber,
    std::shared_ptr<spdlog::logger> lg,
    ArgTypes &&... args
) noexcept
{
    std::string_view fileName = FileName;
    std::string_view funName = FunctionName;
    lg->log(
        Level,
        "{}():{}({}): {}",
        funName.substr(funName.find_last_of(R"(:)") + 1),
        fileName.substr(fileName.find_last_of(R"(\/)") + 1),
        LineNumber,
        fmt::format(std::forward<ArgTypes>(args)...));
}

template<typename T1, typename T2, typename T3, typename T4>
void
KinBase::KinLog::Log(T1 &&Level, T2 &&FileName, T3 &&FunctionName,
        T4 &&LineNumber, std::shared_ptr<spdlog::logger> lg) noexcept
{
    return Log(
        std::forward<T1>(Level),
        std::forward<T2>(FileName),
        std::forward<T3>(FunctionName),
        std::forward<T4>(LineNumber),
        lg,
        ""
    );
}


