#pragma once

//
// Created by Kin on 2018-12-21.
// Copyright © 2018 jihuisoft. All rights reserved.
//
#include "ThreadPool.h"
#include "KinLog.hpp"
#include "KinException.hpp"
#include <memory>
#include <type_traits>

inline KinBase::ThreadPool::ThreadPool(std::size_t numberOfThreads) noexcept
{
    IncreaseThreads(numberOfThreads);
}

inline KinBase::ThreadPool::~ThreadPool() noexcept
{
    RunningFlag = false;

    //Add task to wake up workers
    Tasks.enqueue([] { });
    for(auto &worker : Workers)
    {
        Must(worker.joinable());
        worker.join();
    }
    Workers.clear();
}

inline void KinBase::ThreadPool::IncreaseThreads(std::size_t numbers) noexcept
{
    for(std::size_t i = 0; i != numbers; ++i)
    {
        Workers.emplace_back([this] ()noexcept{
            DoTask();
        });
    }
}

inline std::size_t KinBase::ThreadPool::GetThreadsNumber() const noexcept
{
    return Workers.size();
}

inline void KinBase::ThreadPool::DoTask() noexcept
{
    TaskType task;
    while(RunningFlag)
    {
        try
        {
            Tasks.wait_dequeue(task);
            if(!RunningFlag)
            {
                Tasks.enqueue(std::move(task));
                break;
            }
            task();
        }
        catch(const std::exception& e)
        {
            kInfo("Unbelievable exception in ThreadPool with :{}",e.what());
        }
        catch(...)
        {
            kInfo("Unbelievable exception in ThreadPool with unknown");
        }
    }
}

template <typename T, typename... T2>
std::future <std::invoke_result_t <T, T2...>>
KinBase::ThreadPool::Submit(T &&f, T2 &&... args) noexcept
{
    using ReturnType = std::invoke_result_t <T, T2...>;

    auto task = std::make_shared <std::packaged_task <ReturnType()>>([f,args...] () mutable {
        return std::invoke(f,std::forward<T2>(args)...);
    });

    Tasks.enqueue([task] {
        (*task)();
    });

    return task->get_future();
}

inline KinBase::ThreadPool &KinBase::ThreadPool::DefaultPool() noexcept
{
    static ThreadPool pool{8};
    return pool;
}