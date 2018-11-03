//
// Created by Kin on 2018-11-03.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_KINFILESYSTEM_HPP
#define KINBASE_KINFILESYSTEM_HPP

#include "KinFileSystem.h"

inline std::string_view KinBase::KinFileSystem::GetFileNameFromFullPath(const std::string_view &Path)
{
    return Path.substr(Path.find_last_of(R"(\/)") + 1);
}

#endif //KINBASE_KINFILESYSTEM_HPP
