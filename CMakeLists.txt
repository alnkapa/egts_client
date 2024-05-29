cmake_minimum_required(VERSION 3.5)

project(egtsClient VERSION 0.0.1)

set(CMAKE_BUILD_TYPE "Debug")

add_executable(egts_client main.cpp)

set_target_properties(
    egts_client PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

add_subdirectory(lib)


find_package(GTest REQUIRED)

add_executable(test_endian test.cpp)

target_link_libraries(test_endian GTest::gtest_main)

add_test(reverse test_endian)

enable_testing()