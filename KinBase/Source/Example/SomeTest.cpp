//
// Created by Kin on 2018/12/22.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include <KinBase/ThreadPool.hpp>
#include <KinBase/random.hpp>
#include <KinBase/ObjectTransferTrack.h>
#include <KinBase/ObjectTransferTrack.h>
class myClass
{
public:
    myClass() = default;
    void operator()(int,double){};
    void Run(int,double){};

    int Num;
};

int main()
{
    {

        auto lambda = [ot = KinBase::ObjectTransferTrack{}] {

        };

        getchar();
    }

    getchar();

//    myClass mc{};
//
//    auto smc = std::make_shared<myClass>();
//
//    auto& pool = KinBase::ThreadPool::DefaultPool();
//
//    std::weak_ptr<myClass> wmc = smc;
//    pool.Submit(mc,3,5.0);
//    pool.Submit(wmc,3,5.0);
////
//    pool.Submit(&myClass::Run,mc,3,5.0);
//    pool.Submit(&myClass::Run,smc,3,5.0);
//
//    pool.Submit(&myClass::Num, mc);
//    pool.Submit(&myClass::Num, smc);
    return 0;
}