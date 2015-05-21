cmake_minimum_required(VERSION 3.2)
project(Graphics)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(Graphics ${SOURCE_FILES} Math/Grid.h Math/Vector.h Math/Math.h)