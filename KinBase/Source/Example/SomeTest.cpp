//
// Created by Kin on 2018/12/22.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include <KinBase/ThreadPool.hpp>
#include <KinBase/random.hpp>
#include <KinBase/ObjectTransferTrack.h>

int main()
{
    using namespace KinBase;
    std::atomic_int32_t Result = 0;
    for(int i = 0; i!=1; ++i)
    {
        std::thread([&Result]{
            ObjectTransferTrack ot;
            int n1= 3;
            int n2 = 4;
            auto ft = ThreadPool::DefaultPool().Submit([](int& a, int &b){
                ++a;
                ++b;
            },n1,std::ref(n2));

            ft.wait();

//            auto ft = ThreadPool::DefaultPool().Submit([&Result]{
//                   auto sleepTime = effolkronium::random_static::get(50, 1000);
//                   kInfo("Do Some Work with time {}ms", sleepTime);
//                   std::this_thread::sleep_for(
//                           std::chrono::milliseconds(2000));
//                   return ++Result;
//
//               });



//            std::this_thread::sleep_for(std::chrono::milliseconds(effolkronium::random_static::get(1, 10)));


            kInfo("n1 is: {}, n2 is: {}", n1,n2);

        }).detach();

    }

    getchar();
    kInfo("Result is :{}",Result);
    return 0;
}