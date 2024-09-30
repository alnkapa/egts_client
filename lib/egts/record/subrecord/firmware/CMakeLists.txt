cmake_minimum_required(VERSION 3.5)

add_library(firmware firmware.cpp)

target_include_directories(
    firmware 
    PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/egts
)


if(BUILD_TESTING)
    add_subdirectory(firmware_tests)
endif()
