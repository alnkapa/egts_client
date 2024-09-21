cmake_minimum_required(VERSION 3.5)


add_subdirectory(sr_result_code)
add_subdirectory(sr_record_response)
add_subdirectory(sr_term_identity)

add_library(subrecord subrecord.cpp)

target_include_directories(
    subrecord 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts
)

target_link_libraries(
    subrecord 
    sr_record_response
    sr_result_code
    sr_term_identity
    )

if(BUILD_TESTING)
    add_subdirectory(subrecord_tests)
endif()
