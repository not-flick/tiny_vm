#include "memory.h"

#include <windows.h>
#include <cstring>

void* MemoryManager::Allocate(std::size_t bytes)
{
    return ::VirtualAlloc(
        nullptr,
        bytes,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE);
}

bool MemoryManager::Free(void* address)
{
    return ::VirtualFree(
        address,
        0,
        MEM_RELEASE);
}

bool MemoryManager::Protect(
    void* address,
    std::size_t bytes,
    DWORD protection)
{
    DWORD oldProtect;

    return ::VirtualProtect(
        address,
        bytes,
        protection,
        &oldProtect);
}

bool MemoryManager::Copy(
    void* destination,
    const void* source,
    std::size_t bytes)
{
    std::memcpy(destination, source, bytes);
    return true;
}

bool MemoryManager::Set(
    void* destination,
    int value,
    std::size_t bytes)
{
    std::memset(destination, value, bytes);
    return true;
}

std::size_t MemoryManager::PageSize() const
{
    SYSTEM_INFO info;
    ::GetSystemInfo(&info);

    return info.dwPageSize;
}

bool MemoryManager::Lock(void* address, std::size_t bytes)
{
    return ::VirtualLock(address, bytes);
}

bool MemoryManager::Unlock(void* address, std::size_t bytes)
{
    return ::VirtualUnlock(address, bytes);
}