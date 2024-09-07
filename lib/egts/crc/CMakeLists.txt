cmake_minimum_required(VERSION 3.5)

# Добавьте подкаталог для тестов
add_subdirectory(tests)

add_library(crc INTERFACE .)

