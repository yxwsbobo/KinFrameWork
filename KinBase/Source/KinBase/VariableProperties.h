//
// Created by Kin on 2018/4/23.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_VARIABLEPROPERTIES_H
#define KINBASE_VARIABLEPROPERTIES_H

#include "EventDispatcher.hpp"
#include <any>
#include <boost/noncopyable.hpp>
#include <iostream>


namespace KinBase
{
    template<typename DestType>
    class PropertiesMap
        : public boost::noncopyable
    {
        DestType *Dest;
        std::shared_mutex Mutex;
        std::unordered_map<std::type_index, EventDispatcherBase<DestType>> Dispatchers;
    public:
        explicit PropertiesMap(DestType *dest)
            : Dest{dest}
        {
        }

        EventDispatcherBase<DestType> &GetDispatcher(std::type_index typeIndex)
        {
            std::shared_lock<std::shared_mutex> sharedLock{Mutex};
            auto it = Dispatchers.find(typeIndex);
            if (it != Dispatchers.end())
            {
                return it->second;
            }
            else
            {
                sharedLock.unlock();
                std::unique_lock<std::shared_mutex> uniqueLock{Mutex};
                return Dispatchers.emplace(typeIndex, Dest).first->second;
            }
        }
    };

    /**
     * @brief Thread Safe \b see EventDispatcher
     * @date 2018/4/23
     * @author Kin
     * @copyright Copyright © 2018 jihuisoft. All rights reserved.
     */
    template<typename DestType>
    class VariableProperties
    {
        PropertiesMap<DestType> SetterMap;
        PropertiesMap<DestType> GetterMap;
        std::shared_mutex Mutex;
        std::unordered_map<std::type_index, std::any> Properties;
        DestType *Dest;
    public:
        ///@copydoc EventDispatcher::Connection
        using Connection = EventDispatcher::Connection;
        ///@copydoc EventDispatcher::ListenerType
        using ListenerType = EventDispatcher::ListenerType;
        ///@copydoc EventDispatcher::ListenerGroup
        using ListenerGroup = EventDispatcher::ListenerGroup;
        ///@copydoc EventDispatcher::ListenerPosition
        using ListenerPosition = EventDispatcher::ListenerPosition;

        explicit VariableProperties(DestType *dest)
            : SetterMap{dest}, GetterMap{dest}, Dest{dest}
        {
        }

        template<typename PropertyType, typename ValueType>
        inline int SetProperty(ValueType &&value)
        {
            return SetterMap.GetDispatcher(std::type_index(typeid(PropertyType)))
                            .DispatchEvent(std::forward<ValueType>(value));
        };

        template<typename PropertyType, typename ValueType>
        inline int GetProperty(ValueType &&value)
        {
            return GetterMap.GetDispatcher(std::type_index(typeid(PropertyType)))
                            .DispatchEvent(std::forward<ValueType>(value));
        };

        template<typename PropertyType, typename ValueType = int>
        inline ValueType GetProperty()
        {
            ValueType value;
            GetProperty<PropertyType>(value);
            return value;
        }

        template<typename PropertyType, typename... Args>
        inline Connection RegisterSetter(Args &&... args)
        {
            return SetterMap.GetDispatcher(std::type_index(typeid(PropertyType)))
                            .RegisterEventListener(std::forward<Args>(args) ...);
        }

        template<typename PropertyType, typename TargetType, typename ValueType, typename FunType, typename... Args>
        inline Connection RegisterSetter(FunType fun, Args &&... args)
        {
            std::function<int(TargetType *, ValueType)> listener = fun;
            return SetterMap.GetDispatcher(std::type_index(typeid(PropertyType)))
                            .RegisterEventListener(listener, std::forward<Args>(args) ...);
        };

        template<typename PropertyType, typename ValueType, typename FunType, typename... Args>
        inline Connection RegisterSetter(FunType fun, Args &&... args)
        {
            std::function<int(ValueType)> listener = fun;
            return SetterMap.GetDispatcher(std::type_index(typeid(PropertyType)))
                            .RegisterEventListener(listener, std::forward<Args>(args) ...);
        };


        template<typename PropertyType, typename... Args>
        inline Connection RegisterGetter(Args &&... args)
        {
            return GetterMap.GetDispatcher(std::type_index(typeid(PropertyType)))
                            .RegisterEventListener(std::forward<Args>(args) ...);
        }

        template<typename PropertyType, typename TargetType, typename ValueType, typename FunType, typename... Args>
        inline Connection RegisterGetter(FunType fun, Args &&... args)
        {
            std::function<int(TargetType *, std::add_lvalue_reference_t<ValueType>)> listener = fun;
            return GetterMap.GetDispatcher(std::type_index(typeid(PropertyType)))
                            .RegisterEventListener(listener, std::forward<Args>(args) ...);
        };

        template<typename PropertyType, typename ValueType, typename FunType, typename... Args>
        inline Connection RegisterGetter(FunType fun, Args &&... args)
        {
            std::function<int(std::add_lvalue_reference_t<ValueType>)> listener = fun;
            return GetterMap.GetDispatcher(std::type_index(typeid(PropertyType)))
                            .RegisterEventListener(listener, std::forward<Args>(args) ...);
        };

        template<typename PropertyType>
        inline void InsertProperty(PropertyType &&property)
        {
            std::any value = property;
            std::unique_lock<std::shared_mutex> uniqueLock{Mutex};
            auto it = Properties.insert_or_assign(value.type(), std::move(value));
            typename HasMemberAttach<PropertyType>::type has;
            CallPropertyAttach(has, std::any_cast<std::decay_t<PropertyType>>(&it.first->second), Dest);
        }

        template<typename PropertyType>
        inline void EraseProperty()
        {
            std::unique_lock<std::shared_mutex> uniqueLock{Mutex};
            Properties.erase(std::type_index(typeid(PropertyType)));
        }

    private:
        template<typename PropertyType, typename TargetType>
        static void CallPropertyAttach(std::true_type &, PropertyType property, TargetType target)
        {
            property->PropertyAttach(target);
        }

        template<typename PropertyType, typename TargetType>
        static void CallPropertyAttach(double &, PropertyType property, TargetType target)
        {
            (*property)->PropertyAttach(target);
        }

        template<typename PropertyType, typename TargetType>
        static void CallPropertyAttach(std::false_type &, PropertyType, TargetType)
        {
        }

        template<typename T>
        struct HasMemberAttach
        {
        private:
            template<typename U>
            static auto check(int) -> decltype(std::declval<U>().PropertyAttach(nullptr), std::true_type());

            template<typename U>
            static auto check(int) -> decltype(std::declval<U>()->PropertyAttach(nullptr), double(1.0));

            template<typename U>
            static std::false_type check(...);

        public:
            using type = decltype(check<T>(0));
        };
    };


}
#endif //KINBASE_VARIABLEPROPERTIES_H
