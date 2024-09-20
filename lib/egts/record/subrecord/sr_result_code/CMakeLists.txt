cmake_minimum_required(VERSION 3.5)

add_library(sr_result_code sr_result_code.cpp)

target_include_directories(
    sr_result_code 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts/error
)


if(BUILD_TESTING)
    add_subdirectory(sr_result_code_tests)
endif()
