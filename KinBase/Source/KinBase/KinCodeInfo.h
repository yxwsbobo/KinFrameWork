#include <utility>

//
// Created by Kin on 2018-10-31.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_KINCODEINFO_H
#define KINBASE_KINCODEINFO_H

#include <string>
#include "KinMacroDefine.h"


namespace KinBase
{

    /**
     * @brief
     * @date 2018-10-31
     * @author Kin
     * @copyright Copyright © 2018 jihuisoft. All rights reserved.
     */
    class KinCodeInfo
    {
    public:

        KinCodeInfo(
            const std::string &Message,
            const std::string &Condition,
            const std::string &FileName,
            const std::string &FunctionName,
            const std::string &LineNumber
        );

        virtual ~KinCodeInfo() = default;

    public:

        const std::string &getMessage() const;

        void setMessage(const std::string &Message);

        const std::string &getCondition() const;

        void setCondition(const std::string &Condition);

        const std::string &getFileName() const;

        void setFileName(const std::string &FileName);

        const std::string &getFunctionName() const;

        void setFunctionName(const std::string &FunctionName);

        const std::string &getLineNumber() const;

        void setLineNumber(const std::string &LineNumber);

        const std::string &getFullInfo() const;

        template<typename T1, typename T2, typename T3, typename T4, typename... ArgTypes>
        static KinCodeInfo
        Create(T1 &&Condition, T2 &&FileName, T3 &&FunctionName, T4 &&LineNumber, ArgTypes &&... args) noexcept(false);

        template<typename T1, typename T2, typename T3, typename T4>
        static KinCodeInfo Create(T1 &&Condition, T2 &&FileName, T3 &&FunctionName, T4 &&LineNumber) noexcept(false);

    private:
        std::string Message;
        std::string Condition;
        std::string FileName;
        std::string FunctionName;
        std::string LineNumber;
        std::string FullInfo;
    };

#define MakeCodeInfo(Condition, ...)                                     \
KinBase::KinCodeInfo::Create(                                            \
        Condition,                                                       \
        MACRO_GET_FILE_NAME(),                                           \
        __FUNCTION__,                                                    \
        __LINE__,                                                        \
        ##__VA_ARGS__                                                    \
        )

}
#endif //KINBASE_KINCODEINFO_H
