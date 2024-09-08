cmake_minimum_required(VERSION 3.5)

# Добавьте подкаталог для тестов
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_library(crc INTERFACE .)

