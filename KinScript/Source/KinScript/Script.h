//
// Created by Kin on 2018-09-26.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINSCRIPT_SCRIPT_H
#define KINSCRIPT_SCRIPT_H

#include <string>
#include "EnumDefine.h"


namespace KinScript {
    class Script;

    extern Script CreateEngine(const std::string& FilePath);

    extern Script CreateEngine(EngineType Type);

/**
 * @brief
 * @date 2018-09-26
 * @author Kin
 * @copyright Copyright © 2018 jihuisoft. All rights reserved.
 */
    class Script {
    public:
        ~Script() = default;

        virtual void Load(const std::string& FilePath) = 0;

        virtual void RunCode(const std::string& Code) = 0;

    protected:
        Script() = default;
        friend Script CreateEngine(const std::string& FilePath);
        friend Script CreateEngine(EngineType Type);


    };

}
#endif //KINSCRIPT_SCRIPT_H
