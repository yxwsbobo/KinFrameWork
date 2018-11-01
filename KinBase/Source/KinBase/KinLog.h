//
// Created by Kin on 2018-10-31.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_KINLOG_H
#define KINBASE_KINLOG_H

#include <memory>
#include <string>
#include <spdlog/spdlog.h>

#include "SingleObject.h"

namespace KinBase
{

    class KinLogClass
    {
    public:
        ~KinLogClass() = default;
        std::shared_ptr<spdlog::logger> Logger;

    private:
        std::ios_base::Init InitIO;
        friend SingleObject<KinLogClass>;
        KinLogClass();
    };

    namespace KinLog
    {
        static auto& DefaultLogger = SingleObject<KinLogClass>::Value.Logger;

        template <typename... ArgTypes>
        static std::shared_ptr<spdlog::logger> CreateLogger(ArgTypes &&... args);

        template <typename T1, typename T2, typename T3, typename T4, typename... ArgTypes>
        static void Log(T1 &&Level, T2 &&FileName, T3 &&FunctionName,
            T4 &&LineNumber, ArgTypes &&... args);

        template <typename T1, typename T2, typename T3, typename T4>
        static void Log(T1 &&Level, T2 &&FileName, T3 &&FunctionName, T4 &&LineNumber);

        template <typename T1, typename T2, typename T3, typename T4, typename... ArgTypes>
        static void Log(T1 &&Level, T2 &&FileName, T3 &&FunctionName,
            T4 &&LineNumber,std::shared_ptr<spdlog::logger> lg, ArgTypes &&... args);

        template <typename T1, typename T2, typename T3, typename T4>
        static void Log(T1 &&Level, T2 &&FileName, T3 &&FunctionName,
            T4 &&LineNumber,std::shared_ptr<spdlog::logger> lg);
    }



    //
//    namespace KinLog3
//    {
//        template <typename T1, typename T2, typename T3, typename T4, typename... ArgTypes>
//        static void OutputLogInfo(T1 &&Level, T2 &&FileName, T3 &&FunctionName,
//            T4 &&LineNumber, ArgTypes &&... args);
//
//        template <typename T1, typename T2, typename T3, typename T4, typename... ArgTypes>
//        static void OutputLogInfo(T1 &&Level, T2 &&FileName, T3 &&FunctionName,
//            T4 &&LineNumber,std::shared_ptr<spdlog::logger> lg, ArgTypes &&... args);
//
//        class InitDefaultLogStyle
//        {
//        public:
//            void Run(){};
//        private:
//            template <typename T>
//            friend class DefaultLoggerClass;
//
//            InitDefaultLogStyle();
//        };
//
//        template <typename T>
//        class DefaultLoggerClass
//        {
//        public:
//            template <typename T1, typename T2, typename T3, typename T4, typename... ArgTypes>
//            friend void OutputLogInfo(T1 &&Level, T2 &&FileName, T3 &&FunctionName,
//                T4 &&LineNumber, ArgTypes &&... args);
//
//            template <typename T1, typename T2, typename T3, typename T4, typename... ArgTypes>
//            friend void OutputLogInfo(T1 &&Level, T2 &&FileName, T3 &&FunctionName,
//                T4 &&LineNumber,std::shared_ptr<spdlog::logger> lg, ArgTypes &&... args);
//            friend InitDefaultLogStyle;
//            static InitDefaultLogStyle InitStyleMember;
//            static std::shared_ptr<spdlog::logger> DefaultLogger;
//        };
//
//        template <typename T>
//        KinLog::InitDefaultLogStyle DefaultLoggerClass<T>::InitStyleMember{};
//
//        template <typename T>
//        std::shared_ptr<spdlog::logger> DefaultLoggerClass<T>::DefaultLogger = spdlog::stdout_color_mt("log");
//
//        using DefaultInfo = DefaultLoggerClass<void>;
//
//        InitDefaultLogStyle::InitDefaultLogStyle()
//        {

//        }
//
//    }





#define kLogHelp(Level, ...)                                              \
KinBase::KinLog::Log(                                          \
        Level,                                                            \
        MACRO_GET_FILE_NAME(),                                           \
        __FUNCTION__,                                                    \
        __LINE__,                                                        \
        ##__VA_ARGS__                                                    \
        )


#define kTrace(...) kLogHelp(spdlog::level::trace, ##__VA_ARGS__)
#define kDebug(...) kLogHelp(spdlog::level::debug, ##__VA_ARGS__)
#define kInfo(...) kLogHelp(spdlog::level::info, ##__VA_ARGS__)
#define kWarn(...) kLogHelp(spdlog::level::warn, ##__VA_ARGS__)
#define kError(...) kLogHelp(spdlog::level::err, ##__VA_ARGS__)

}
#endif //KINBASE_KINLOG_H
