cmake_minimum_required(VERSION 3.10)

# Укажите путь к Boost
# find_package(Boost REQUIRED COMPONENTS system)

# Укажите путь к Qt
# find_package(Qt5 REQUIRED COMPONENTS Widgets)

# Терминальная версия
add_library(
    terminal
    ${CMAKE_SOURCE_DIR}/terminal/*.cpp    
)

target_link_libraries(
    terminal
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# # Qt версия
# add_executable(QtApp
#     src/qt/main.cpp
#     src/qt/NetworkModel.cpp
#     src/qt/NetworkController.cpp
#     src/qt/NetworkView.h
#     src/qt/NetworkModel.h
#     src/qt/NetworkController.h
# )

# target_link_libraries(QtApp Qt5::Widgets)
