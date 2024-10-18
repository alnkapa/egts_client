cmake_minimum_required(VERSION 3.10)

# Укажите путь к Boost
find_package(Boost REQUIRED)


# Терминальная версия
add_library(
    terminal 
    STATIC
    network_client_model.cpp
    file_model.cpp
)

