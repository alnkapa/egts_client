cmake_minimum_required(VERSION 3.5)

add_library(record record.cpp)

# target_include_directories(
#     record 
#     PUBLIC 
#     ${CMAKE_SOURCE_DIR}/lib/egts/error
#     ${CMAKE_SOURCE_DIR}/lib/egts/crc
# )


# if(BUILD_TESTING)
#     add_subdirectory(transport_tests)
# endif()
