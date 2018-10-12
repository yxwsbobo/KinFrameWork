//
// Created by Kin on 2018-10-12.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef SYSTEMCONTROL_INPUTSTRUCTINFO_H
#define SYSTEMCONTROL_INPUTSTRUCTINFO_H

#include <KinBase/BetterEnum.h>

namespace SystemControl {

BETTER_ENUM(InputStructType, int,
            UnValid,
            MouseMove,
            LeftButtonDown,
            LeftButtonUp,
            RightButtonDown,
            RightButtonUp,
            MiddleButtonDown,
            MiddleButtonUp,
            KeyboardDown,
            KeyboardUp);

struct MouseStructInfo
{
    InputStructType Type = InputStructType::UnValid;
    int x = 0;
    int y = 0;
    bool IsAbs = true;
};

struct KeyboardStructInfo
{
    InputStructType Type = InputStructType::UnValid;
    int vkCode = 0;
};

}

#endif //SYSTEMCONTROL_INPUTSTRUCTINFO_H
