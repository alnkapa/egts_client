cmake_minimum_required(VERSION 3.5)

add_library(transport transport.cpp)

target_include_directories(
    transport 
    PUBLIC
    ${CMAKE_SOURCE_DIR}/lib/egts
)

if(BUILD_TESTING)
    add_subdirectory(transport_tests)
endif()
