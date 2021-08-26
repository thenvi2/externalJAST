cmake_minimum_required(VERSION 3.20)
project(JAST)

set(CMAKE_CXX_STANDARD 14)

add_executable(JAST main.cpp JAST.h JAST.cpp FileIO.cpp FileIO.h)
