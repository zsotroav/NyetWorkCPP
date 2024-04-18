cmake_minimum_required(VERSION 3.27)
project(NetServer)

set(CMAKE_CXX_STANDARD 17)

add_executable(NetServer main.cpp)

if(WIN32)
    target_link_libraries(NetServer wsock32 ws2_32)
endif()
