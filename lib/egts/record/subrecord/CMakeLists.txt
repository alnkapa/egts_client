cmake_minimum_required(VERSION 3.5)


add_subdirectory(sr_record_response)

add_library(subrecord subrecord.cpp)

target_include_directories(
    subrecord 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts
)
# sr_record_response

# target_link_libraries(subrecord sr_record_response)

if(BUILD_TESTING)
    add_subdirectory(subrecord_tests)
endif()
