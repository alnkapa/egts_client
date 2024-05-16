cmake_minimum_required(VERSION 3.5)

# file(GLOB transportSRC
#     "*/*.cpp"
#     "*/*.hpp"
#     "*.cpp"
#     "*.hpp"
# )

add_library(transport INTERFACE)
target_include_directories(transport INTERFACE .)