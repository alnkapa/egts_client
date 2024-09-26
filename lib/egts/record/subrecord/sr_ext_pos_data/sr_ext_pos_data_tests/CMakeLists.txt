cmake_minimum_required(VERSION 3.5)

find_package(GTest REQUIRED)

add_executable(sr_ext_pos_data_tests sr_ext_pos_data_tests.cpp)

target_include_directories(
    sr_ext_pos_data_tests 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts
)

target_link_libraries(sr_ext_pos_data_tests GTest::GTest GTest::Main egts)

add_test(NAME EGTS_SR_EXT_POS_DATA COMMAND sr_ext_pos_data_tests)
