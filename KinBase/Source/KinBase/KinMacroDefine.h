//
// Created by Kin on 2018/9/23 0023.
// Copyright © 2018 jihuisoft. All rights reserved.
//

#ifndef KINBASE_KINMACRODEFINE_H
#define KINBASE_KINMACRODEFINE_H

#include <string>

#define MACRO_GET_FILE_NAME() \
std::string(__FILE__).substr(std::string(__FILE__).find_last_of("/\\") + 1)

#endif //KINBASE_KINMACRODEFINE_H
