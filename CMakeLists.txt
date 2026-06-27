cmake_minimum_required(VERSION 3.20)

project(TinyVM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TinyVM
add_executable(TinyVM
    Src/TinyVM/main.cpp
)

# TinyShell
add_executable(TinyShell
    Src/TinyShell/main.cpp
)

target_include_directories(TinyShell PRIVATE
    Src/TinyShell
)

if(MSVC)
    target_compile_options(TinyShell PRIVATE /utf-8)
endif()