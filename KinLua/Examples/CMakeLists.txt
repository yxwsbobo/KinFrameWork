
project(Example)

find_package(KinLua)

add_executable(Example main.cpp)

target_link_libraries(Example ${KinLua_LIBRARIES})