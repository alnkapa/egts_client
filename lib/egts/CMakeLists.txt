cmake_minimum_required(VERSION 3.5)

add_subdirectory(crc)
add_subdirectory(error)
add_subdirectory(transport)


add_library(egts INTERFACE .)



