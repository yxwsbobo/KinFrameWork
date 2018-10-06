//
// Created by Kin on 2018/4/30.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_OBJECT_H
#define KINBASE_OBJECT_H

#include <memory>
#include "VariableProperties.h"

namespace KinBase
{

/**
 * @brief
 * @date 2018/4/30
 * @author Kin
 * @copyright Copyright © 2018 jihuisoft. All rights reserved.
 */
    class Object : public EventDispatcherBase<Object>,
            public VariableProperties<Object>,
            public std::enable_shared_from_this<Object>
    {
    public:
        Object()
                : EventDispatcherBase{this}, VariableProperties{this}
        {
        }
        virtual ~Object() = default;

    };

}
#endif //KINBASE_OBJECT_H
