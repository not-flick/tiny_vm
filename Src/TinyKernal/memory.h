#pragma once

#include <cstddef>
#include <cstdint>

class MemoryManager
{
public:
    void* Allocate(std::size_t bytes);
    bool Free(void* address);

    bool Protect(void* address,
                 std::size_t bytes,
                 uint32_t protection);

    bool Copy(void* destination,
              const void* source,
              std::size_t bytes);

    bool Set(void* destination,
             int value,
             std::size_t bytes);

    bool Lock(void* address,  std::size_t bytes);
    bool Unlock(void* address, std::size_t bytes);

    std::size_t PageSize() const;
};