cmake_minimum_required(VERSION 3.5)

project(egtsClient VERSION 0.0.1)

set(CMAKE_BUILD_TYPE "Debug")
include(CTest)
enable_testing()

set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BOOST_ROOT_PREFIX ${CMAKE_SOURCE_DIR}/lib/boost)
set(CMAKE_PREFIX_PATH ${BOOST_ROOT_PREFIX})

# Найдите Boost
find_package(Boost REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
else()
    message(FATAL_ERROR "Boost not found")
endif()


add_subdirectory(lib/egts/transport)

add_executable(egts_client main.cpp)


target_include_directories(
    egts_client 
    PRIVATE 
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(
    egts_client 
    PRIVATE 
    transport 
    ${Boost_LIBRARIES}
)

add_executable(1 1.cpp)

target_include_directories(
    1 
    PRIVATE 
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(
    1
    PRIVATE 
    transport 
    ${Boost_LIBRARIES}
)


# find_package(Boost REQUIRED COMPONENTS system)

# target_link_libraries(egts_client transport)


# add_subdirectory(lib)


# if(BUILD_TESTING)
#   # ... CMake code to create tests ...
#     find_package(GTest REQUIRED)
#     add_executable(test_endian test.cpp)
#     target_link_libraries(test_endian GTest::gtest_main)
#     add_test(reverse test_endian)
#     enable_testing()
# endif()
