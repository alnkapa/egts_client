cmake_minimum_required(VERSION 3.5)

add_library(crc INTERFACE .)

if(BUILD_TESTING)
    add_subdirectory(crc_tests)
endif()

