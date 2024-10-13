cmake_minimum_required(VERSION 3.10)

# Укажите путь к Boost
find_package(Boost REQUIRED)


# Терминальная версия
add_library(
    terminal 
    STATIC
    network_model.cpp
)

target_link_libraries(
    terminal
    PUBLIC
    .
)
