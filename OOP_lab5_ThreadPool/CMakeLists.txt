cmake_minimum_required(VERSION 3.8)
project(OOP_lab5_ThreadPool)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp Factorizator.cpp PollardRho.cpp Factorizator.hpp PollardRho.hpp ThreadPool.hpp ConcurrentSystem.hpp)
add_executable(OOP_lab5_ThreadPool ${SOURCE_FILES})