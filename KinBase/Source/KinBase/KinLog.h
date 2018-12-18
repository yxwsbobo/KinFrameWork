//
// Created by Kin on 2018-10-31.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_KINLOG_H
#define KINBASE_KINLOG_H

#include <memory>
#include <string>
#include <spdlog/spdlog.h>


namespace KinBase
{
    namespace KinLog
    {

        std::shared_ptr<spdlog::logger> &GetDefaultLogger() noexcept;

        void SetDefaultLogger(const std::shared_ptr<spdlog::logger> &Logger) noexcept;

        template<typename... ArgTypes>
        static std::shared_ptr<spdlog::logger> CreateLogger(ArgTypes &&... args);

        template<typename T1, typename T2, typename T3, typename T4, typename... ArgTypes>
        static void Log(
            T1 &&Level, T2 &&FileName, T3 &&FunctionName, T4 &&LineNumber, ArgTypes &&... args
        );

        template<typename T1, typename T2, typename T3, typename T4>
        static void Log(T1 &&Level, T2 &&FileName, T3 &&FunctionName, T4 &&LineNumber);

        template<typename T1, typename T2, typename T3, typename T4, typename... ArgTypes>
        static void Log(
            T1 &&Level,
            T2 &&FileName,
            T3 &&FunctionName,
            T4 &&LineNumber,
            std::shared_ptr<spdlog::logger> lg,
            ArgTypes &&... args
        );

        template<typename T1, typename T2, typename T3, typename T4>
        static void Log(
            T1 &&Level, T2 &&FileName, T3 &&FunctionName, T4 &&LineNumber, std::shared_ptr<spdlog::logger> lg
        );

        class KinDefaultLoggerWarpper
        {
        private:
            friend std::shared_ptr<spdlog::logger> &GetDefaultLogger() noexcept;

            std::shared_ptr<spdlog::logger> Logger;

            KinDefaultLoggerWarpper();

            ~KinDefaultLoggerWarpper() = default;
        };
    }


#define __Kin_LogHelp(Level, ...)                                              \
KinBase::KinLog::Log(                                          \
        Level,                                                            \
        __FILE__,                                           \
        __FUNCTION__,                                                    \
        __LINE__,                                                        \
        ##__VA_ARGS__                                                    \
        )


#define kTrace(...) __Kin_LogHelp(spdlog::level::trace, ##__VA_ARGS__)
#define kDebug(...) __Kin_LogHelp(spdlog::level::debug, ##__VA_ARGS__)
#define kInfo(...) __Kin_LogHelp(spdlog::level::info, ##__VA_ARGS__)
#define kWarn(...) __Kin_LogHelp(spdlog::level::warn, ##__VA_ARGS__)
#define kError(...) __Kin_LogHelp(spdlog::level::err, ##__VA_ARGS__)

}
#endif //KINBASE_KINLOG_H
