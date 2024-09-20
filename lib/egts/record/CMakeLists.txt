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
