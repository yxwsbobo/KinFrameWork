#pragma once

//
// Created by Kin on 2018-12-21.
// Copyright © 2018 jihuisoft. All rights reserved.
//
#include <concurrentqueue/blockingconcurrentqueue.h>
#include <functional>

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
        explicit ThreadPool(std::size_t Size = 4);

        ~ThreadPool() = default;

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&& pool);


        template <typename T,typename ...ArgTypes>
        void Invoke(T&& fun,ArgTypes&&... args)
        {
            auto task = [=]{
                return fun(std::forward<ArgTypes>(args)...);
            };


        }

    private:
        std::size_t MaxThreads;
        std::size_t CurrentThreads;
        moodycamel::BlockingConcurrentQueue<std::function<void(void)>> TaskQueue;
    };

}
