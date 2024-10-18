cmake_minimum_required(VERSION 3.10)


include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(terminal)
add_subdirectory(gui)

# add_library(modules .)

# target_include_directories(
#     modules
#     INTERFACE
#     ${CMAKE_SOURCE_DIR}/include
#     .
# )

# target_link_libraries(
#     modules
#     PRIVATE
#     ${CMAKE_SOURCE_DIR}/lib
# )
