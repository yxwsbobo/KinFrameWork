
cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

find_package(spdlog)
find_package(KinBoost)
find_package(concurrentqueue)

add_library(KinBase empty.cpp KinException.h KinMacroDefine.h BetterEnum.h EventDispatcher.h Object.h TemplateHelp.h VariableProperties.h KinException.hpp EventDispatcher.hpp KinCodeInfo.h KinCodeInfo.hpp KinLog.h KinLog.hpp random.hpp ObjectTransferTrack.h ThreadPool.h ThreadPool.hpp KinBase.h)