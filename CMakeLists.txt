cmake_minimum_required(VERSION 3.20)

project(TinyVM LANGUAGES CXX RC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TinyVM
add_executable(TinyVM
    Src/TinyVM/main.cpp
)

# TinyShell
add_executable(TinyShell
    Src/TinyShell/main.cpp
    Src/TinyShell/shell.cpp
    Src/TinyShell/parser.cpp
    Src/TinyShell/filesystem.cpp
    Src/TinyShell/commands.cpp
    Src/TinyShell/Commands/help.cpp
    Src/TinyShell/Commands/clear.cpp
    Src/TinyShell/Commands/exit.cpp
    Src/TinyShell/Commands/pwd.cpp
    Src/TinyShell/Commands/cd.cpp
    Src/TinyShell/Commands/ls.cpp
    Src/TinyShell/Commands/mkdir.cpp
    Src/TinyShell/Commands/touch.cpp
    Src/TinyShell/Commands/rm.cpp
    Src/TinyShell/Commands/rmdir.cpp
    Src/TinyShell/Commands/cat.cpp
    Src/TinyShell/Commands/echo.cpp
    Src/TinyShell/Commands/whoami.cpp
    Src/TinyShell/Commands/hostname.cpp
    Src/TinyShell/Commands/version.cpp
    Src/TinyShell/TinyShell.rc
)

target_include_directories(TinyShell PRIVATE
    Src/TinyShell
)

add_custom_command(TARGET TinyShell POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Src/TinyShell/logo/TinyShell.ico"
        "$<TARGET_FILE_DIR:TinyShell>/TinyShell.ico"
)

if(MSVC)
    target_compile_options(TinyShell PRIVATE /utf-8)
endif()
