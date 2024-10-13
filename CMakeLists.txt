cmake_minimum_required(VERSION 3.10)

project(egtsClient VERSION 0.0.1)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # clangd


add_subdirectory(lib/modules)

add_executable(egts_client main.cpp)


target_include_directories(
    egts_client
    PRIVATE
    modules    
)

target_link_libraries(
    egts_client
    PRIVATE
    modules  
)
