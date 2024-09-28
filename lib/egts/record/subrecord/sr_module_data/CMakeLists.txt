cmake_minimum_required(VERSION 3.5)

add_library(sr_module_data sr_module_data.cpp)

target_include_directories(
    sr_module_data 
    PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/egts
)


if(BUILD_TESTING)
    add_subdirectory(sr_module_data_tests)
endif()
