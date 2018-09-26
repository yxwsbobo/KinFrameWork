//
// Created by Kin on 2018-09-26.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#include "Script.h"
#include <KinBase/KinException.h>

KinScript::Script KinScript::CreateEngine(const std::string &FilePath) {
    auto Index = FilePath.find_last_of('.');
    std::string EngineName;
    bool IsFile = true;

    if(Index == std::string::npos)
    {
        IsFile = false;
        EngineName = FilePath;
    }
    else
    {
        EngineName = FilePath.substr(Index + 1);
    }

    KinScript::Script Result;

    if(EngineName == "lua")
    {
        Result = CreateEngine(EngineType::Lua);
    }
    else if(EngineName == "chai")
    {
        Result = CreateEngine(EngineType::ChaiScript);
    }

    if(IsFile)
    {
        Result.Load(FilePath);
    }

}

KinScript::Script KinScript::CreateEngine(EngineType Type) {
    return Script();
}