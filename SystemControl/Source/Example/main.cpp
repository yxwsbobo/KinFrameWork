//
// Created by Kin on 2018-10-12.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "../SystemControl/KinControl.h"
#include <thread>
#include <chrono>
#include <Windows.h>

int main()
{
    using namespace SystemControl;

    KinControl kc;

    MouseStructInfo ms;

    auto fScreenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1;
    auto fScreenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1;
    ms.x = 200<<16;
    ms.y = 200<<16;
    ms.x /= fScreenWidth;
    ms.y /= fScreenHeight;
    ms.Type = InputStructType::MouseMove;

    getchar();
    kc.MouseEvent(ms);

    KeyboardStructInfo ks;

    getchar();

    Sleep(5000);

    ks.Type = InputStructType::KeyboardDown;
    ks.vkCode = 'A';
    kc.KeyboardEvent(ks);
    ks.Type = InputStructType::KeyboardUp;
    kc.KeyboardEvent(ks);

    return 0;
}