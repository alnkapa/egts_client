cmake_minimum_required(VERSION 3.5)

add_library(sr_pos_data sr_pos_data.cpp)

target_include_directories(
    sr_pos_data 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts
)


if(BUILD_TESTING)
    add_subdirectory(sr_pos_data_tests)
endif()
