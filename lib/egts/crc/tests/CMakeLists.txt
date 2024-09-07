cmake_minimum_required(VERSION 3.5)

# Укажите, что это подкаталог для тестов
enable_testing()

# Найдите Google Test
find_package(GTest REQUIRED)

# Создайте исполняемый файл для тестов
add_executable(crc_tests crc_tests.cpp)

# Укажите зависимости для тестов
target_link_libraries(crc_tests GTest::GTest GTest::Main)
