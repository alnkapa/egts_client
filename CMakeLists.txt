cmake_minimum_required(VERSION 3.5)

project(egtsClient VERSION 0.0.1)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(lib)

add_executable(egts_client main.cpp)

# find_package(Boost REQUIRED COMPONENTS system)

# target_link_libraries(egts_client ${Boost_LIBRARIES})


# add_subdirectory(lib)


# if(BUILD_TESTING)
#   # ... CMake code to create tests ...
#     find_package(GTest REQUIRED)
#     add_executable(test_endian test.cpp)
#     target_link_libraries(test_endian GTest::gtest_main)
#     add_test(reverse test_endian)
#     enable_testing()
# endif()