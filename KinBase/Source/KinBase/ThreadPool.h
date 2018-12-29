#pragma once

//
// Created by Kin on 2018-12-21.
// Copyright © 2018 jihuisoft. All rights reserved.
//
#include <concurrentqueue/blockingconcurrentqueue.h>
#include <functional>
#include <atomic>
#include <vector>
#include <future>
#include "KinBase.h"

namespace KinBase
{

    /**
     * @brief
     * @date 2018-12-21
     * @author Kin
     * @copyright Copyright © 2018 jihuisoft. All rights reserved.
     */
    class ThreadPool
    {
    public:
        using TaskType = std::function <void()>;

        static ThreadPool &DefaultPool() noexcept;

    public:
        explicit ThreadPool(std::size_t numberOfThreads) noexcept;

        ~ThreadPool() noexcept;


        ThreadPool(const ThreadPool &) = delete;
        ThreadPool &operator=(const ThreadPool &) = delete;

        ThreadPool(ThreadPool &&pool) = delete;
        ThreadPool &operator=(ThreadPool &&pool) = delete;


        void IncreaseThreads(std::size_t numbers) noexcept;

        std::size_t GetThreadsNumber() const noexcept;

        template <typename T, typename... T2>
        decltype(auto) Submit(T &&f, T2 &&... args) noexcept;

    private:
        void DoTask() noexcept;

    private:
        std::atomic_bool RunningFlag = true;
        moodycamel::BlockingConcurrentQueue <TaskType> Tasks;
        std::vector <std::thread> Workers;
    };

}
