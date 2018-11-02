//
// Created by Kin on 2018/4/21.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_TEMPLATEHELP_H
#define KINBASE_TEMPLATEHELP_H

#include <typeindex>


namespace KinBase
{

    template<typename Type>
    struct GetTypeIndex
    {
        template<typename T>
        static std::type_index GetIndex(typename T::element_type *)
        {
            return typeid(T::element_type);
        }

        template<typename T>
        static std::type_index GetIndex(...)
        {
            return typeid(T);
        }

        static std::type_index GetIndex()
        {
            return GetIndex<Type>(0);
        }
    };
}

#endif //KINBASE_TEMPLATEHELP_H
