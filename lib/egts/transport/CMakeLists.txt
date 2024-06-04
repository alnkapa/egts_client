cmake_minimum_required(VERSION 3.5)

add_library(transport INTERFACE)

target_include_directories(transport INTERFACE .)

include(CTest)

if(BUILD_TESTING)
  # ... CMake code to create tests ...
    find_package(GTest REQUIRED)
    add_executable(test_transport test_transport.cpp)
    target_link_libraries(test_transport GTest::gtest_main)
    add_test(reverse test_transport)
    enable_testing()
endif()