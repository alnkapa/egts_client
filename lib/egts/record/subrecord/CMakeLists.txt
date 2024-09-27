cmake_minimum_required(VERSION 3.5)


add_subdirectory(sr_result_code)
add_subdirectory(sr_record_response)
add_subdirectory(sr_term_identity)
add_subdirectory(sr_pos_data)
add_subdirectory(sr_ext_pos_data)
add_subdirectory(sr_module_data)

add_library(subrecord subrecord.cpp)

target_include_directories(
    subrecord 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/egts
)

target_link_libraries(
    subrecord 
    PUBLIC
    sr_record_response
    sr_result_code
    sr_term_identity
    sr_pos_data
    sr_ext_pos_data
    sr_module_data
    )

if(BUILD_TESTING)
    add_subdirectory(subrecord_tests)
endif()
