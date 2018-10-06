//
// Created by Kin on 2018/4/21.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "EventDispatcher.h"

int KinBase::EventDispatcher::Dispatch(std::any &event)
{
    return Dispatch(std::type_index(event.type()), event);
}

int KinBase::EventDispatcher::Dispatch(std::any &&event)
{
    std::any e = std::move(event);
    return Dispatch(e);
}

int KinBase::EventDispatcher::Dispatch(std::type_index typeIndex, std::any &event)
{
    std::shared_lock <std::shared_mutex> sharedLock{Mutex};
    auto it = Dispatchers.find(typeIndex);
    if(it != Dispatchers.end())
    {
        auto &handler = it->second;
        sharedLock.unlock();

        if(!handler.empty())
        {
            return handler(event).get();
        }
    }
    return 0;
}

int KinBase::EventDispatcher::Dispatch(std::type_index typeIndex, std::any &&event)
{
    std::any e = std::move(event);
    return Dispatch(typeIndex, e);
}

KinBase::EventDispatcher::Connection
KinBase::EventDispatcher::AddListener(std::type_index typeIndex,
                                      KinBase::EventDispatcher::ListenerType listener,
                                      KinBase::EventDispatcher::ListenerGroup group,
                                      KinBase::EventDispatcher::ListenerPosition position)
{
    std::shared_lock <std::shared_mutex> sharedLock{Mutex};
    auto it = Dispatchers.find(typeIndex);
    if(it != Dispatchers.end())
    {
        auto &handler = it->second;
        sharedLock.unlock();
        return handler.connect(group, listener, position);
    }
    else
    {
        sharedLock.unlock();
        std::unique_lock <std::shared_mutex> uniqueLock{Mutex};
        auto &handler = Dispatchers[typeIndex];
        uniqueLock.unlock();
        return handler.connect(group, listener, position);
    }
}


