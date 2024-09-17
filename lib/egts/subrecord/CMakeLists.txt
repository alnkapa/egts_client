cmake_minimum_required(VERSION 3.5)

add_library(subrecord subrecord.cpp)

target_include_directories(
    subrecord 
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/lib/egts/error
)


if(BUILD_TESTING)
    add_subdirectory(subrecord_tests)
endif()
