#pragma once

#include <cstdint>
#include <windows.h>
#include <string>

struct Process
{
    DWORD pid{};
    HANDLE handle{};
};

class ProcessManager {
public:
    Process Launch(const std::string& executable);
    bool Kill(Process& process);
    void Sleep(uint32_t ms);
    void Exit(int code);
};