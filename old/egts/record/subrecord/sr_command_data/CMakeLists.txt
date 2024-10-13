cmake_minimum_required(VERSION 3.5)

add_library(sr_command_data sr_command_data.cpp)

target_include_directories(
    sr_command_data 
    PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/egts
)


if(BUILD_TESTING)
    add_subdirectory(sr_command_data_tests)
endif()
