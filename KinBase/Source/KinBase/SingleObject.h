//
// Created by Kin on 2018-11-01.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_SINGLEOBJECT_H
#define KINBASE_SINGLEOBJECT_H
namespace KinBase
{
    template <typename T>
    class SingleObject
    {
    public:
        ~SingleObject() = default;
        static T Value;

    private:
        SingleObject() = default;
    };

    template <typename T>
    T SingleObject<T>::Value;

}
#endif //KINBASE_SINGLEOBJECT_H
