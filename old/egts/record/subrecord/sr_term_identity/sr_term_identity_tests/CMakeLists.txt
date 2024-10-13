cmake_minimum_required(VERSION 3.5)

find_package(GTest REQUIRED)

add_executable(sr_term_identity_tests sr_term_identity_tests.cpp)

target_include_directories(
    sr_term_identity_tests 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts
)

target_link_libraries(sr_term_identity_tests GTest::GTest GTest::Main egts)

add_test(NAME SR_TERM_IDENTITY_PARSE COMMAND sr_term_identity_tests)

