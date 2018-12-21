#pragma once

//
// Created by Kin on 2018-12-21.
// Copyright © 2018 jihuisoft. All rights reserved.
//
#include "ThreadPool.h"


KinBase::ThreadPool::ThreadPool(size_t Size)
:MaxThreads{Size},CurrentThreads{0}
{

}
