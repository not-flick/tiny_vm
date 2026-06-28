#pragma once

#include "filesystems.h"
#include "memory.h"
#include "process.h"

#include <iostream>
#include <string>

class KernelLogger
{
public:
    void BootMessage(const std::string& component, const std::string& message)
    {
        Log(component, message);
    }

    void Success(const std::string& message)
    {
        Log("OK", message);
    }

    void Warning(const std::string& message)
    {
        Log("WARN", message);
    }

    void Error(const std::string& message)
    {
        Log("ERR", message);
    }

private:
    void Log(const std::string& component, const std::string& message)
    {
        std::string tag = component;
        if (tag == "OK")
        {
            tag = " OK ";
        }
        else
        {
            if (tag.length() < 4)
            {
                tag.append(4 - tag.length(), ' ');
            }
        }
        std::cout << "[" << tag << "] " << message << "\n";
    }
};

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
    KernelLogger logger;

    void Boot();
    void Shutdown();
};
