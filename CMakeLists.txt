cmake_minimum_required(VERSION 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # clangd
# Укажите имя проекта
project(MyQtProject VERSION 1.0 LANGUAGES CXX)

# Укажите, что мы будем использовать Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Найдите необходимые модули Qt
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# Укажите исходные файлы
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
)

# Укажите заголовочные файлы
set(HEADERS
    src/mainwindow.h
)

# Создайте исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Свяжите исполняемый файл с библиотеками Qt
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

# Укажите стандарт C++
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)


# cmake_minimum_required(VERSION 3.10)

# project(egtsClient VERSION 0.0.1)

# set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # clangd


# add_subdirectory(lib)

# add_executable(egts_client main.cpp)


# # target_include_directories(
# #     egts_client
# #     PRIVATE
# #     modules    
# # )

# # target_link_libraries(
# #     egts_client
# #     PRIVATE
# #     modules  
# # )
