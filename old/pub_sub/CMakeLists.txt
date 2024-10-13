cmake_minimum_required(VERSION 3.5)

add_library(pub_sub INTERFACE)

target_include_directories(
    pub_sub
    INTERFACE    
    .
)
