cmake_minimum_required(VERSION 3.5)


add_subdirectory(subrecord)

add_library(record record.cpp)

target_include_directories(
    record 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts/error
)

target_link_libraries(record subrecord)

if(BUILD_TESTING)
    add_subdirectory(record_tests)
endif()

#    ${CMAKE_SOURCE_DIR}/lib/egts/error
#    ${CMAKE_SOURCE_DIR}/lib/egts/crc
message(" CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(" CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}/lib/egts/error")
message(" CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}/lib/egts/crc")
get_target_property(INCLUDE_DIRS transport INCLUDE_DIRECTORIES)
message(STATUS "Include directories for my_library: ${INCLUDE_DIRS}")
