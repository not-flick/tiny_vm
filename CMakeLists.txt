cmake_minimum_required(VERSION 3.20)

project(TinyVM LANGUAGES CXX RC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(TinyShell)

enable_language(RC)


# ==========================
# TinyKernel Library
# ==========================
add_library(TinyKernal
    Src/TinyKernal/kernal.cpp
    Src/TinyKernal/process.cpp
    Src/TinyKernal/memory.cpp
    Src/TinyKernal/filesystems.cpp
    Src/TinyKernal/syscall.cpp
)

add_library(TVM_64
        Src/TVM_64/cpu.cpp
        Src/TVM_64/ISA.cpp
        Src/TVM_64/ISA.h
        Src/TVM_64/register.cpp
        Src/TVM_64/register.h
        Src/TVM_64/decoder.cpp
        Src/TVM_64/opcode.h
        Src/TVM_64/instruction.h
)

target_link_libraries(TVM_64 PRIVATE TinyKernal)
target_include_directories(TVM_64 PRIVATE
        Src/TinyKernal
)


add_executable(TinyEdit
    Src/TinyEdit/main.cpp
        Src/TinyEdit/main.cpp
        Src/TinyEdit/main.h
)

target_include_directories(TinyKernal PUBLIC
    Src/TinyKernal
)

# ==========================
# TinyVM
# ==========================
add_executable(TinyVM
    Src/TinyVM/main.cpp
)

target_link_libraries(TinyVM PRIVATE
    TinyKernal
)

# ==========================
# TinyShell
# ==========================
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
    Src/TinyShell/Commands/history.cpp
    Src/TinyShell/Commands/alias.cpp
    Src/TinyShell/Commands/unalias.cpp
    Src/TinyShell/Commands/set.cpp
    Src/TinyShell/Commands/unset.cpp
    Src/TinyShell/TinyShell.rc
        Src/TinyShell/resource.h
)

target_include_directories(TinyShell PRIVATE
    Src/TinyShell
)

target_link_libraries(TinyShell PRIVATE
    TinyKernal
)

add_custom_command(TARGET TinyShell POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Src/TinyShell/logo/TinyShell.ico"
        "$<TARGET_FILE_DIR:TinyShell>/TinyShell.ico"
)

if(MSVC)
    target_compile_options(TinyShell PRIVATE /utf-8)
endif()