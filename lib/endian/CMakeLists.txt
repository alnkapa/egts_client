cmake_minimum_required(VERSION 3.5)

project(Endian)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(endian INTERFACE)
target_include_directories(endian INTERFACE .)

