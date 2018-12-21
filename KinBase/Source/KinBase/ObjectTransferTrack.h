#pragma once

//
// Created by Kin on 2018-12-21.
// Copyright © 2018 jihuisoft. All rights reserved.
//
#include "KinLog.hpp"


namespace KinBase
{

    /**
     * @brief
     * @date 2018-12-21
     * @author Kin
     * @copyright Copyright © 2018 jihuisoft. All rights reserved.
     */
    class ObjectTransferTrack
    {
    public:
        ObjectTransferTrack()
        {
            kInfo("In Default Constructor");
        };

        ~ObjectTransferTrack()
        {
            if(flag)
            {
                kInfo("In Destructor");
            }
            else
            {
                kInfo("In Destructor and be stolen");
            }
        }

        ObjectTransferTrack(const ObjectTransferTrack& obj)
        {
            kInfo("In Copy Constructor");
            flag = obj.flag;
        }

        ObjectTransferTrack(ObjectTransferTrack&& obj) noexcept
        {
            kInfo("In Move Constructor");
            flag = obj.flag;
            obj.flag = 0;
        }

        ObjectTransferTrack&operator=(const ObjectTransferTrack& obj)
        {
            kInfo("In Assignment");
            flag = obj.flag;
            return *this;
        }

        ObjectTransferTrack&operator=(ObjectTransferTrack&& obj) noexcept
        {
            kInfo("In Move Assignment");
            flag = obj.flag;
            obj.flag =0;
            return *this;
        }

    private:
        int flag = 1;
    };

}
