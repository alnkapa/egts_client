cmake_minimum_required(VERSION 3.5)

find_package(GTest REQUIRED)

add_executable(sr_pos_data_tests sr_pos_data_tests.cpp)

target_include_directories(
    sr_pos_data_tests 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts
)

target_link_libraries(sr_pos_data_tests GTest::GTest GTest::Main sr_pos_data)

# add_test(NAME PARSE COMMAND sr_pos_data_tests)

