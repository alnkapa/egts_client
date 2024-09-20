cmake_minimum_required(VERSION 3.5)

add_library(sr_record_response sr_record_response.cpp)

target_include_directories(
    sr_record_response 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts/record/subrecord
    ${CMAKE_SOURCE_DIR}/lib/egts/error
)


if(BUILD_TESTING)
    add_subdirectory(sr_record_response_tests)
endif()
