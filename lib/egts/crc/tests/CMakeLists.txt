cmake_minimum_required(VERSION 3.5)

enable_testing()

add_executable(crc_tests crc_tests.cpp)

target_include_directories(crc_tests PRIVATE ${CMAKE_SOURCE_DIR}/lib/egts/crc)


find_package(GTest REQUIRED)

target_link_libraries(crc_tests GTest::GTest GTest::Main)

add_test(NAME CRC8Test COMMAND crc_tests)

add_test(NAME CRC16Test COMMAND crc_tests)

include(CTest)
