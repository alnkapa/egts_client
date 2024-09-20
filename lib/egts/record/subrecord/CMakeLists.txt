cmake_minimum_required(VERSION 3.5)

add_library(subrecord subrecord.cpp)

target_include_directories(
    subrecord 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts/error
)

# target_link_libraries(subrecord record)

if(BUILD_TESTING)
    add_subdirectory(subrecord_tests)
endif()
