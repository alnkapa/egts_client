cmake_minimum_required(VERSION 3.5)

find_package(GTest REQUIRED)

add_executable(subrecord_tests subrecord_tests.cpp)

target_include_directories(
    subrecord_tests 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts
)

target_link_libraries(subrecord_tests GTest::GTest GTest::Main egts)

add_test(NAME PARSE_SUBRECORD COMMAND subrecord_tests)

add_test(NAME PARSE1_SUBRECORD COMMAND subrecord_tests)
