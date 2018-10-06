//
// Created by Kin on 2018/4/21.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_EVENTDISPATCHER_H
#define KINBASE_EVENTDISPATCHER_H

#include <any>
#include <shared_mutex>
#include <unordered_map>
#include <boost/signals2.hpp>
#include <functional>
#include "KinBase.h"
namespace KinBase
{
    struct HandlePosition
    {
        static constexpr int First = -0x7FFFFF00;
        static constexpr int Last = 0x7FFFFF00;
    };
    /**
     * @brief Dispatch any event.
     * @note ThreadSafe only self.Not recommended call Dispatch in multi threads with the same event type.\n
     * If must to do it,make sure the handler thread safe.\b ep:
     * @code{.cpp}
     *
     * //this example in multi threads is safe
     * //one thread call
     * dispatcher.Dispatch(SomeEvent{});
     * //other thread call
     * dispatcher.Dispatch(OtherEvent{});
     *
     *
     * //next example in multi threads may be not safe,is safe or not depend on the handler.who call AddListener
     * //with EventType SomeEvent.the handler must also thread safe.it is Easy to make mistakes.
     * //Better way is don't call Dispatch when the EventType is same in multi thread.
     *
     * //one thread call
     * dispatcher.Dispatch(SomeEvent{});
     * //other thread call
     * dispatcher.Dispatch(SomeEvent{});
     *
     * int handler(SomeEvent& e)
     * {
     *     int a =0;
     *     ++a;//local var is safe
     *     ++global; //will UB
     *     e.change(); // UB if not thread safe
     *     return 0;
     * }
     *
     * @endcode
     *
     * @date 2018/4/21
     * @author Kin
     * @copyright Copyright © 2018 jihuisoft. All rights reserved.
     */
    class EventDispatcher
    {
        using HandlerType = boost::signals2::signal <int(std::any &)>;
        std::shared_mutex Mutex;
        std::unordered_map <std::type_index, HandlerType> Dispatchers;
    public:
        ///@brief Control Connection state
        using Connection = boost::signals2::connection;
        ///@brief Value can be at_back, at_front
        using ListenerPosition = boost::signals2::connect_position;
        ///@brief Call sequence by value,smallest first
        using ListenerGroup = HandlerType::group_type;

        ///@return 0 is go on,other code will stop dispatching Todo this
        using ListenerType = std::function <int(std::any &)>;

        /**
         * @brief Send event to object,it will dispatch event to Handler.\n
         * The event type based on init type.\b ep:\n
         * @code{.cpp}
         *
         * class child :public base{};
         * auto ch = std::make_shared<child>();
         * // std::any event = ch;//Handler need handle event with type std::shared_ptr<child>
         * std::any event = std::dynamic_pointer_cast<base>(ch);// Handler need handle event with type std::shared_ptr<base>
         * dispatcher.Dispatch(event);
         *
         * @endcode
         * @note ThreadSafe see @refitem EventDispatcher
         * @param event NoLimit just make sure the Event type is same to receive event type \n
         * @see @refitem EventDispatcher::AddListener \n
         * @refitem EventDispatcher::ListenerType
         * @return return zero is go on,other words is stop reason
         */
        int Dispatch(std::any &event);

        ///Todo when listener return !zero stop dispatch
        ///Todo dispatch with pointer
        int Dispatch(std::any &&event);

        template <typename EventType>
        inline int Dispatch(EventType &&event)
        {
            std::any e = const_cast<std::decay_t <EventType> *>(&event);
            return Dispatch(std::type_index(typeid(std::decay_t <EventType>)), e);
        }

        template <typename EventType>
        static inline std::decay_t<EventType>* GetEventPointer(std::any &event)
        {
            auto Result = std::any_cast<std::decay_t<EventType> *>(&event);
            if(Result)
            {
                return *Result;
            }

            auto Result2 = std::any_cast<std::decay_t<EventType>>(&event);
            if(Result2)
            {
                return Result2;
            }

            Must(0);
            return nullptr;
        }

        /**
         * @brief Allow to Dispatch Event to other eventType\n
         * Not suggest use this direct,Because tye typeIndex may different to event type.
         * Handler Must receive event use event type same with Dispatched event type
         * @param event The event info
         * @param typeIndex The event type
         * @return \b see Dispatch(std::any&)
         */
        int Dispatch(std::type_index typeIndex, std::any &event);

        ///@copydoc Dispatch(std::type_index,std::any&)
        int Dispatch(std::type_index typeIndex, std::any &&event);

        /**
         * @brief not suggest use this too.
         * @see EventDispatcher::Dispatch
         */
        Connection AddListener(std::type_index typeIndex,
                               ListenerType listener,
                               ListenerGroup group = 0,
                               ListenerPosition position = ListenerPosition::at_back);


        template <typename EventType>
        inline Connection AddListener(std::function <int(EventType)> listener,
                               ListenerGroup group = 0,
                               ListenerPosition position = ListenerPosition::at_back)
        {
            auto lambda = [listener](std::any &event) {
                auto e = GetEventPointer<EventType>(event);
                return listener(*e);
            };
            return AddListener(std::type_index(typeid(std::decay_t<EventType>)), lambda, group, position);
        }

        template <typename EventType, typename HandlerType>
        inline Connection AddListener(std::weak_ptr <HandlerType> handler,
                                                int (HandlerType::*listener)(EventType),
                                                ListenerGroup group = 0,
                                                ListenerPosition position = ListenerPosition::at_back)
        {
            auto lambda = [handler,listener](std::any &event) {
                if(auto hd = handler.lock())
                {
                    auto e = GetEventPointer<EventType>(event);
                    return ((*hd).*listener)(*e);
                }
                //Todo Erase this listener
                return 0;
            };
            return AddListener(std::type_index(typeid(std::decay_t<EventType>)), lambda, group, position);
        };

        template<typename EventType, typename HandlerType>
        inline Connection AddListener(std::weak_ptr<HandlerType> handler,
                                      void (HandlerType::*listener)(EventType),
                                      ListenerGroup group = 0,
                                      ListenerPosition position = ListenerPosition::at_back)
        {
            auto lambda = [handler, listener](std::any &event) {
                if(auto hd = handler.lock())
                {
                    auto e = GetEventPointer<EventType>(event);
                    ((*hd).*listener)(*e);
                    return 0;
                }
                //Todo Erase this listener
                return 0;
            };
            return AddListener(std::type_index(typeid(std::decay_t<EventType>)), lambda, group, position);
        };

        template <typename EventType, typename HandlerType>
        inline Connection AddListener(std::shared_ptr <HandlerType> handler,
                                      int (HandlerType::*listener)(EventType),
                                      ListenerGroup group = 0,
                                      ListenerPosition position = ListenerPosition::at_back)
        {
            std::weak_ptr <HandlerType> weakPtr = handler;
            return AddListener(weakPtr, listener, group, position);
        }

        template<typename EventType, typename HandlerType>
        inline Connection AddListener(std::shared_ptr<HandlerType> handler,
                                      void (HandlerType::*listener)(EventType),
                                      ListenerGroup group = 0,
                                      ListenerPosition position = ListenerPosition::at_back)
        {
            std::weak_ptr<HandlerType> weakPtr = handler;
            return AddListener(weakPtr, listener, group, position);
        }

        template <typename EventType>
        inline Connection AddListener(int (*listener)( EventType),
                                                ListenerGroup group = 0,
                                                ListenerPosition position = ListenerPosition::at_back)
        {
            auto lambda = [listener](std::any &event) {
                auto e = GetEventPointer<EventType>(event);
                return (*listener)(*e);
            };
            return AddListener(std::type_index(typeid(std::decay_t<EventType>)), lambda, group, position);
        }

        template<typename EventType>
        inline Connection AddListener(void (*listener)(EventType),
                                      ListenerGroup group = 0,
                                      ListenerPosition position = ListenerPosition::at_back)
        {
            auto lambda = [listener](std::any &event) {
                auto e = GetEventPointer<EventType>(event);
                (*listener)(*e);
                return 0;
            };
            return AddListener(std::type_index(typeid(std::decay_t<EventType>)), lambda, group, position);
        }

    };

    /**
     * @brief Provide an easy-to-use interface for EventDispatch.
     *
     * -----------------------
     * ## Four way to receive event. ##
     * 1. BySelf
     * 2. member function , the object must be smart pointer,so the caller can know the member function is exist.
     * 3. static function
     * 4. std::function //in this case,not allowed auto convert,it means you must send a std::function object to arg \n
     *   or call AddEventListener with template param
     *
        Type          |   Example
        ------------- | -------------
        BySelf           | RegisterEventListener(&Self::listener)
        Member Function  | target.RegisterEventListener(Obj,&Obj::listener)
        Static Function  | target.RegisterEventListener(&listener)
        std::function    | target.RegisterEventListener<targetType,EventType>(listener)
        std::function    | target.RegisterEventListener<EventType>(listener)

     *
     * ### Te Return type can be void or int ###
     *
     * --------------------
     *
     * @copydetails EventDispatcher
     */
    template <typename DestType>
    class EventDispatcherBase
    {
        DestType *Dest;
        EventDispatcher Dispatcher;
    public:

        ///@copydoc EventDispatcher::Connection
        using Connection = EventDispatcher::Connection;
        ///@copydoc EventDispatcher::ListenerType
        using ListenerType = EventDispatcher::ListenerType;
        ///@copydoc EventDispatcher::ListenerGroup
        using ListenerGroup = EventDispatcher::ListenerGroup;
        ///@copydoc EventDispatcher::ListenerPosition
        using ListenerPosition = EventDispatcher::ListenerPosition;

        explicit EventDispatcherBase(DestType *Target)
                : Dest{Target}
        {
        }

        template <typename... Args>
        inline int DispatchEvent(Args&& ...args)
        {
            return Dispatcher.Dispatch(std::forward<Args>(args) ...);
        }

        template <typename... Args>
        inline Connection RegisterEventListener(Args&& ...args)
        {
            return Dispatcher.AddListener(std::forward<Args>(args) ...);
        }

        ///@brief General function base on std::function
        template <typename TargetType, typename EventType>
        inline Connection RegisterEventListener(std::function <int(TargetType *, EventType)> listener,
                                                ListenerGroup group = 0,
                                                ListenerPosition position = ListenerPosition::at_back)
        {
            auto target = dynamic_cast<TargetType *>(Dest);
            Must(target);
            auto lambda = [target,listener](std::any &event) {
                auto e = EventDispatcher::GetEventPointer<EventType>(event);
                return listener(target,*e);
            };
            return Dispatcher.AddListener(std::type_index(typeid(std::decay_t<EventType>)),
                                          lambda, group, position);
        }

        template<typename EventType>
        inline Connection RegisterEventListener(std::function<int(EventType)> listener,
                                                ListenerGroup group = 0,
                                                ListenerPosition position = ListenerPosition::at_back)
        {
            auto lambda = [listener](std::any &event) {
                auto e = EventDispatcher::GetEventPointer<EventType>(event);
                return listener(*e);
            };
            return Dispatcher.AddListener(std::type_index(typeid(std::decay_t<EventType>)),
                                          lambda, group, position);
        }


        template <typename TargetType, typename EventType, typename HandlerType>
        inline Connection RegisterEventListener(std::shared_ptr <HandlerType> handler,
                                                int (HandlerType::*listener)(TargetType *, EventType),
                                                ListenerGroup group = 0,
                                                ListenerPosition position = ListenerPosition::at_back)
        {
            std::weak_ptr <HandlerType> weakPtr = handler;
            return RegisterEventListener(weakPtr, listener, group, position);
        };

        template<typename TargetType, typename EventType, typename HandlerType>
        inline Connection RegisterEventListener(std::shared_ptr<HandlerType> handler,
                                                void (HandlerType::*listener)(TargetType *, EventType),
                                                ListenerGroup group = 0,
                                                ListenerPosition position = ListenerPosition::at_back)
        {
            std::weak_ptr<HandlerType> weakPtr = handler;
            return RegisterEventListener(weakPtr, listener, group, position);
        };

        template <typename TargetType, typename EventType, typename HandlerType>
        inline Connection RegisterEventListener(std::weak_ptr <HandlerType> handler,
                                                int (HandlerType::*listener)(TargetType *, EventType),
                                                ListenerGroup group = 0,
                                                ListenerPosition position = ListenerPosition::at_back)
        {
            auto target = dynamic_cast<TargetType *>(Dest);
            Must(target);

            auto lambda = [target,handler, listener](std::any &event) {
                if(auto hd = handler.lock())
                {
                    auto e = EventDispatcher::GetEventPointer<EventType>(event);
                    return ((*hd).*listener)(target,*e);
                }
                //Todo Erase this listener
                return 0;
            };
            return Dispatcher.AddListener(std::type_index(typeid(std::decay_t<EventType>)),
                                          lambda, group, position);
        };

        template<typename TargetType, typename EventType, typename HandlerType>
        inline Connection RegisterEventListener(std::weak_ptr<HandlerType> handler,
                                                void (HandlerType::*listener)(TargetType *, EventType),
                                                ListenerGroup group = 0,
                                                ListenerPosition position = ListenerPosition::at_back)
        {
            auto target = dynamic_cast<TargetType *>(Dest);
            Must(target);

            auto lambda = [target, handler, listener](std::any &event) {
                if(auto hd = handler.lock())
                {
                    auto e = EventDispatcher::GetEventPointer<EventType>(event);
                    ((*hd).*listener)(target, *e);
                }
                //Todo Erase this listener
                return 0;
            };
            return Dispatcher.AddListener(std::type_index(typeid(std::decay_t<EventType>)),
                                          lambda, group, position);
        };

        template <typename TargetType, typename EventType>
        inline Connection RegisterEventListener(int (*listener)(TargetType *, EventType),
                                                ListenerGroup group = 0,
                                                ListenerPosition position = ListenerPosition::at_back)
        {
            auto target = dynamic_cast<TargetType *>(Dest);
            Must(target);

            auto lambda = [target,listener](std::any &event) {
                auto e = EventDispatcher::GetEventPointer<EventType>(event);
                return (*listener)(target,*e);
            };
            return Dispatcher.AddListener(std::type_index(typeid(std::decay_t<EventType>)),
                                          lambda, group, position);
        };

        template<typename TargetType, typename EventType>
        inline Connection RegisterEventListener(void (*listener)(TargetType *, EventType),
                                                ListenerGroup group = 0,
                                                ListenerPosition position = ListenerPosition::at_back)
        {
            auto target = dynamic_cast<TargetType *>(Dest);
            Must(target);

            auto lambda = [target, listener](std::any &event) {
                auto e = EventDispatcher::GetEventPointer<EventType>(event);
                (*listener)(target, *e);
                return 0;
            };
            return Dispatcher.AddListener(std::type_index(typeid(std::decay_t<EventType>)),
                                          lambda, group, position);
        };

        template <typename EventType, typename SelfType>
        inline Connection RegisterEventListener(int (SelfType::*listener)(EventType),
                                                ListenerGroup group = 0,
                                                ListenerPosition position = ListenerPosition::at_back)
        {
            auto self = dynamic_cast<SelfType *>(Dest);
            Must(self);

            auto lambda = [self, listener](std::any &event) {
                auto e = EventDispatcher::GetEventPointer<EventType>(event);
                return (self->*listener)(*e);
            };
            return Dispatcher.AddListener(std::type_index(typeid(std::decay_t<EventType>)),
                                          lambda, group, position);
        }

        template<typename EventType, typename SelfType>
        inline Connection RegisterEventListener(void (SelfType::*listener)(EventType),
                                                ListenerGroup group = 0,
                                                ListenerPosition position = ListenerPosition::at_back)
        {
            auto self = dynamic_cast<SelfType *>(Dest);
            Must(self);

            auto lambda = [self, listener](std::any &event) {
                auto e = EventDispatcher::GetEventPointer<EventType>(event);
                (self->*listener)(*e);
                return 0;
            };
            return Dispatcher.AddListener(std::type_index(typeid(std::decay_t<EventType>)),
                                          lambda, group, position);
        }

    };

}
#endif //KINBASE_EVENTDISPATCHER_H
