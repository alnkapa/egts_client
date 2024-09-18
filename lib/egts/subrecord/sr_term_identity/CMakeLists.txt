cmake_minimum_required(VERSION 3.5)

add_library(sr_term_identity sr_term_identity.cpp)

target_include_directories(
    sr_term_identity 
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/lib/egts/error
)


if(BUILD_TESTING)
    add_subdirectory(sr_term_identity_tests)
endif()
