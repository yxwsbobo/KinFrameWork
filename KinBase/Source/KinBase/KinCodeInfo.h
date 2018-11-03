//
// Created by Kin on 2018-10-31.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_KINCODEINFO_H
#define KINBASE_KINCODEINFO_H

#include <string_view>
#include <string>

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
            const std::string_view &Condition,
            const std::string_view &FileName,
            const std::string_view &FunctionName,
            int LineNumber
        );

        virtual ~KinCodeInfo() = default;

    public:
        const std::string &getFullInfo() const;

        const std::string &getMessage() const;

        void setMessage(const std::string &Message);

        const std::string_view &getCondition() const;

        void setCondition(const std::string_view &Condition);

        const std::string_view getFileName() const;

        const std::string_view& getFullFileName() const;

        void setFileName(const std::string_view &FileName);

        const std::string_view &getFunctionName() const;

        void setFunctionName(const std::string_view &FunctionName);

        int getLineNumber() const;

        void setLineNumber(int LineNumber);

        template<typename T1, typename T2, typename T3, typename T4, typename... ArgTypes>
        static KinCodeInfo
        Create(T1 &&Condition, T2 &&FileName, T3 &&FunctionName, T4 &&LineNumber, ArgTypes &&... args) noexcept(false);

        template<typename T1, typename T2, typename T3, typename T4>
        static KinCodeInfo Create(T1 &&Condition, T2 &&FileName, T3 &&FunctionName, T4 &&LineNumber) noexcept(false);

    private:
        std::string Message;
        std::string_view Condition;
        std::string_view FileName;
        std::string_view FunctionName;
        int LineNumber;
        mutable std::string FullInfo;
    };

#define MakeCodeInfo(Condition, ...)                                     \
KinBase::KinCodeInfo::Create(                                            \
        Condition,                                                       \
        __FILE__,                                           \
        __FUNCTION__,                                                    \
        __LINE__,                                                        \
        ##__VA_ARGS__                                                    \
        )

}
#endif //KINBASE_KINCODEINFO_H
