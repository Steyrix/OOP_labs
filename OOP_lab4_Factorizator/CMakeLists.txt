cmake_minimum_required(VERSION 3.8)
project(OOP_lab4_Factorizator)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp Headers/Factorizator.h Headers/PollardRho.h Headers/MyException.h Sources/Factorizator.cpp Sources/PollardRho.cpp)
add_executable(OOP_lab4_Factorizator ${SOURCE_FILES})