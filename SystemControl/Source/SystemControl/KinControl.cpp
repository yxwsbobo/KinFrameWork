//
// Created by Kin on 2018-10-12.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "KinControl.h"
#include <Windows.h>

void SystemControl::KinControl::MouseEvent(const SystemControl::MouseStructInfo &Info) {
    if(Info.Type == +InputStructType::UnValid)
    {
        return;
    }
    INPUT ip{};
    ip.type = INPUT_MOUSE;

    ip.mi.dx = Info.x;
    ip.mi.dy = Info.y;
    if(Info.Type == +InputStructType::MouseMove)
    {
        ip.mi.dwFlags = MOUSEEVENTF_MOVE;
    }
    else if(Info.Type == +InputStructType::LeftButtonDown)
    {
        ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    }
    else if(Info.Type == +InputStructType::LeftButtonUp)
    {
        ip.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    }
    else if(Info.Type == +InputStructType::RightButtonDown)
    {
        ip.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    }
    else if(Info.Type == +InputStructType::RightButtonUp)
    {
        ip.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    }
    else if(Info.Type == +InputStructType::MiddleButtonDown)
    {
        ip.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
    }
    else if(Info.Type == +InputStructType::MiddleButtonUp)
    {
        ip.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
    }

    if(Info.IsAbs)
    {
        ip.mi.dwFlags |= MOUSEEVENTF_ABSOLUTE;
    }
    SendInput(1,&ip, sizeof(ip));
}
void SystemControl::KinControl::KeyboardEvent(const SystemControl::KeyboardStructInfo &Info) {
    if(Info.Type == +InputStructType::UnValid)
    {
        return;
    }

    INPUT ip{};
    ip.type = INPUT_KEYBOARD;

    ip.ki.wVk = static_cast<WORD>(Info.vkCode);
    ip.ki.wScan = static_cast<WORD>(MapVirtualKeyA(static_cast<UINT>(Info.vkCode), MAPVK_VK_TO_VSC));

    if(Info.Type == +InputStructType::KeyboardUp)
    {
        ip.ki.dwFlags = KEYEVENTF_KEYUP;
    }
    SendInput(1,&ip, sizeof(ip));
}
