

set(spdlog_INCLUDE_DIRS $ENV{KinBase}/spdlog/include)

if(KIN_EASY_USE_LIB)
include_directories(${spdlog_INCLUDE_DIRS})
endif(KIN_EASY_USE_LIB)
