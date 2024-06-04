cmake_minimum_required(VERSION 3.5)

project(egtsClient VERSION 0.0.1)

include(CTest)

set(CMAKE_BUILD_TYPE "Debug")

add_executable(egts_client main.cpp)

set_target_properties(
    egts_client PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

find_package(Boost REQUIRED COMPONENTS context)

target_link_libraries(egts_client PRIVATE Boost::context)

add_subdirectory(lib)


# if(BUILD_TESTING)
#   # ... CMake code to create tests ...
#     find_package(GTest REQUIRED)
#     add_executable(test_endian test.cpp)
#     target_link_libraries(test_endian GTest::gtest_main)
#     add_test(reverse test_endian)
#     enable_testing()
# endif()