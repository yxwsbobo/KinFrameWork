
cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

add_library(KinBase empty.cpp KinException.h KinMacroDefine.h BetterEnum.h EventDispatcher.h Object.h TemplateHelp.h VariableProperties.h KinException.hpp EventDispatcher.hpp KinCodeInfo.h KinCodeInfo.hpp KinLog.h KinLog.hpp KinFileSystem.h KinFileSystem.hpp random.hpp)