//
// Created by Kin on 2018-11-03.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_KINFILESYSTEM_H
#define KINBASE_KINFILESYSTEM_H

#include <string_view>

namespace KinBase
{

    /**
     * @brief
     * @date 2018-11-03
     * @author Kin
     * @copyright Copyright © 2018 jihuisoft. All rights reserved.
     */
    class KinFileSystem
    {
    public:
        KinFileSystem() = default;

        ~KinFileSystem() = default;

        static std::string_view GetFileNameFromFullPath(const std::string_view& Path);
    };

}
#endif //KINBASE_KINFILESYSTEM_H
