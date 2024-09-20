cmake_minimum_required(VERSION 3.5)

add_library(subrecord subrecord.cpp)

target_include_directories(
    subrecord 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts
)

target_link_libraries(subrecord record)

if(BUILD_TESTING)
    add_subdirectory(subrecord_tests)
endif()


#    ${CMAKE_SOURCE_DIR}/lib/egts/error
#    ${CMAKE_SOURCE_DIR}/lib/egts/crc
# message(" CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
# message(" CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}/lib/egts/error")
# message(" CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}/lib/egts/crc")
# get_target_property(INCLUDE_DIRS transport INCLUDE_DIRECTORIES)
# message(STATUS "Include directories for my_library: ${INCLUDE_DIRS}")
