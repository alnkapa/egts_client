cmake_minimum_required(VERSION 3.5)

add_subdirectory(crc)
add_subdirectory(error)
add_subdirectory(transport)
add_subdirectory(record)


add_library(egts INTERFACE)

target_include_directories(
    egts
    INTERFACE
    crc
    error
    transport
    record
    .
)

target_link_libraries(
    egts
    INTERFACE
    transport
    record
)
