#include "process.h"

Process ProcessManager::Launch(const std::string& executable)
{
    STARTUPINFOA si{};
    PROCESS_INFORMATION pi{};

    si.cb = sizeof(si);

    Process process{};

    if (::CreateProcessA(
        executable.c_str(),
        nullptr,
        nullptr,
        nullptr,
        FALSE,
        0,
        nullptr,
        nullptr,
        &si,
        &pi))
    {
        process.pid = pi.dwProcessId;
        process.handle = pi.hProcess;

        CloseHandle(pi.hThread);
    }

    return process;
}

bool ProcessManager::Kill(Process& process)
{
    if (!process.handle)
        return false;

    BOOL success = ::TerminateProcess(process.handle, 0);

    CloseHandle(process.handle);
    process.handle = nullptr;

    return success == TRUE;
}

void ProcessManager::Sleep(uint32_t ms)
{
    ::Sleep(ms);
}

void ProcessManager::Exit(int code)
{
    ::ExitProcess(code);
}