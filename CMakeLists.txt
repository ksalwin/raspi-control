cmake_minimum_required(VERSION 3.30)

project(raspi-control)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(raspi-control main/main.cpp)
