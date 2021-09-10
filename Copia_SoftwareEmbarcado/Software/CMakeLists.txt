cmake_minimum_required(VERSION 3.19)
project(FAN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
set(CMAKE_CXX_STANDARD 11)

add_executable(FAN mainn.cpp /home/felipe/CLionProjects/FAN/RemyHVH250.cpp)

