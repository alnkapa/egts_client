cmake_minimum_required(VERSION 3.5)

find_package(GTest REQUIRED)

add_executable(sr_record_response_tests sr_record_response_tests.cpp)

target_include_directories(
    sr_record_response_tests 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts
)

target_link_libraries(sr_record_response_tests GTest::GTest GTest::Main egts)

add_test(NAME SR_RECORD_RESPONSE_PARSE COMMAND sr_record_response_tests)

