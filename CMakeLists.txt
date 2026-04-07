cmake_minimum_required(VERSION 3.16)
project(eil_sample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(eil_sample
    src/main.cpp
    src/eil_model.cpp
)

target_include_directories(eil_sample PRIVATE include)
