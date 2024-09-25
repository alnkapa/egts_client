cmake_minimum_required(VERSION 3.5)

find_package(GTest REQUIRED)

add_executable(transport_tests transport_tests.cpp)

target_include_directories(
    transport_tests 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts
)

target_link_libraries(transport_tests GTest::GTest GTest::Main egts)

add_test(NAME TRANSPORT_PARSE_HEADER COMMAND transport_tests)

add_test(NAME TRANSPORT_PARSE_FRAME COMMAND transport_tests)


add_test(NAME TRANSPORT_SEND_RECEIVE COMMAND transport_tests)

add_test(NAME TRANSPORT_SEND_RECEIVE_1 COMMAND transport_tests)

add_test(NAME TRANSPORT_FULL COMMAND transport_tests)

add_test(NAME TRANSPORT_PARSE_BUFFER COMMAND transport_tests)
