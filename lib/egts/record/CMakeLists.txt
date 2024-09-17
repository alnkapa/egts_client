cmake_minimum_required(VERSION 3.5)

add_library(record record.cpp)

target_include_directories(
    record 
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/lib/egts/error
)


if(BUILD_TESTING)
    add_subdirectory(record_tests)
endif()
