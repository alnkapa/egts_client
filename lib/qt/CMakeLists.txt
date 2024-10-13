cmake_minimum_required(VERSION 3.10)

# Укажите путь к Qt
find_package(Qt5 REQUIRED COMPONENTS Network)


# Qt версия
add_library(
    qt 
    STATIC
    network_model.cpp
)

target_link_libraries(
    qt
    PUBLIC
    .
)
target_link_libraries(
    qt 
    PRIVATE 
    Qt5::Network
)
