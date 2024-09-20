cmake_minimum_required(VERSION 3.5)

find_package(GTest REQUIRED)

add_executable(record_tests record_tests.cpp)

target_include_directories(
    record_tests 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts
)

target_link_libraries(record_tests GTest::GTest GTest::Main egts)

add_test(NAME RECORD_PARSE COMMAND record_tests)

add_test(NAME RECORD_PARSE1 COMMAND record_tests)
