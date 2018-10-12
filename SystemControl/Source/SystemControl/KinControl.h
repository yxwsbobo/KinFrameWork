//
// Created by Kin on 2018-10-12.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef SYSTEMCONTROL_KINCONTROL_H
#define SYSTEMCONTROL_KINCONTROL_H

#include "InputStructInfo.h"

namespace SystemControl {

/**
 * @brief
 * @date 2018-10-12
 * @author Kin
 * @copyright Copyright © 2018 jihuisoft. All rights reserved.
 */
class KinControl
{
 public:
    KinControl() = default;
    ~KinControl() = default;

    void MouseEvent(const MouseStructInfo& Info);
    void KeyboardEvent(const KeyboardStructInfo& Info);
};

}
#endif //SYSTEMCONTROL_KINCONTROL_H
