cmake_minimum_required(VERSION 3.5)

find_package(GTest REQUIRED)

add_executable(firmware_tests firmware_tests.cpp)

target_include_directories(
    firmware_tests 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts
)

target_link_libraries(firmware_tests GTest::GTest GTest::Main egts)

add_test(NAME FIRMWARE_OBJECT_DATA_HEADER COMMAND firmware_tests)

