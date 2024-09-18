cmake_minimum_required(VERSION 3.5)

find_package(GTest REQUIRED)

add_executable(sr_result_code_tests sr_result_code_tests.cpp)

target_include_directories(
    sr_result_code_tests 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts/error
    ${CMAKE_SOURCE_DIR}/lib/egts/subrecord/sr_result_code
)

target_link_libraries(sr_result_code_tests GTest::GTest GTest::Main sr_result_code)

add_test(NAME PARSE COMMAND sr_result_code_tests)

