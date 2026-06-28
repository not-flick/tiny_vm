#pragma once

#include "filesystems.h"
#include "memory.h"
#include "process.h"

#include <iostream>
#include <string>

class Console
{
public:
    void Print(const std::string& text)
    {
        std::cout << text;
    }

    void Clear()
    {
        std::cout << "\x1B[2J\x1B[H";
    }
};

class TinyKernel
{
public:

    Console console;
    FileSystem filesystem;
    ProcessManager process;
    MemoryManager memory;

    void Boot();
    void Shutdown();
};
