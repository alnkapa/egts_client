cmake_minimum_required(VERSION 3.10)

# Укажите путь к Qt
find_package(Qt5 REQUIRED COMPONENTS Network Core)


# Qt версия
add_library(
    qt 
    STATIC
    network_client_model.cpp
    file_model.cpp
)

target_link_libraries(
    qt 
    PRIVATE 
    Qt5::Network
    Qt5::Core
)
