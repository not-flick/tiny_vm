#pragma once

#include "filesystems.h"
#include "memory.h"
#include "process.h"

#include <windows.h>

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
        std::cout.flush();
    }

    void Clear()
    {
        const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
        if (output == INVALID_HANDLE_VALUE)
        {
            return;
        }

        CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo{};
        if (!GetConsoleScreenBufferInfo(output, &screenBufferInfo))
        {
            return;
        }

        const COORD home{0, 0};
        DWORD written = 0;
        const DWORD totalCells = static_cast<DWORD>(screenBufferInfo.dwSize.X * screenBufferInfo.dwSize.Y);

        FillConsoleOutputCharacterW(output, L' ', totalCells, home, &written);
        FillConsoleOutputAttribute(output, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, totalCells, home, &written);
        SetConsoleCursorPosition(output, home);
        std::cout.flush();
    }
};

class TinyKernel
{
public:

    Console console;
    FileSystem filesystem;
    ProcessManager process;
    MemoryManager memory;
    void* ramBase{nullptr};
    KernelLogger logger;

    void Boot();
    void Shutdown();
};
