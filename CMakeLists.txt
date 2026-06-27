


cmake_minimum_required(VERSION 3.20)

project(TinyVM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Put executable inside Build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Collect every .cpp file under Src/
file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/Src/*.cpp"
)

add_executable(TinyVM ${SOURCES})

target_include_directories(TinyVM PRIVATE
    ${PROJECT_SOURCE_DIR}/Include
    ${PROJECT_SOURCE_DIR}/Src
)

target_compile_definitions(TinyVM PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)