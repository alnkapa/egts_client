cmake_minimum_required(VERSION 3.5)

project(egtsClient VERSION 0.0.1)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # clangd
include(CTest)
enable_testing()

set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BOOST_ROOT_PREFIX ${CMAKE_SOURCE_DIR}/lib/boost)
set(CMAKE_PREFIX_PATH ${BOOST_ROOT_PREFIX})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/nmea.txt
    ${CMAKE_CURRENT_BINARY_DIR}/nmea.txt
    COPYONLY
)

find_package(Boost REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
else()
    message(FATAL_ERROR "Boost not found")
endif()


add_subdirectory(lib/egts)
add_subdirectory(lib/pub_sub)
add_subdirectory(lib/nmea)

add_executable(egts_client main.cpp my_reader.cpp queue.cpp my_file_reader.cpp)

# auth.cpp queue.cpp


target_include_directories(
    egts_client 
    PRIVATE
    egts
    pub_sub
    nmea
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(
    egts_client 
    PRIVATE
    egts
    pub_sub
    nmea
    ${Boost_LIBRARIES}
)
