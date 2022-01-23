cmake_minimum_required(VERSION 3.1)
include_directories(headers)
project(Balancer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra")

add_executable(Balancer main.cpp sources/config.cpp sources/configReader.cpp sources/udpReceiver.cpp sources/udpSender.cpp)
