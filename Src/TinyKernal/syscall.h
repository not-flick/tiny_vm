#pragma once

#include <string>
#include <vector>

class TinyKernel;

enum class SyscallID
{
    PRINT,
    CLEAR,

    CREATE_FILE,
    DELETE_FILE,

    CREATE_DIRECTORY,
    DELETE_DIRECTORY,

    READ_FILE,
    WRITE_FILE,

    FILE_EXISTS,

    RUN_PROCESS,
    KILL_PROCESS,

    ALLOCATE_MEMORY,
    FREE_MEMORY,

    EXIT
};

class SyscallDispatcher
{
public:
    explicit SyscallDispatcher(TinyKernel& kernel);

    bool Handle(
        SyscallID id,
        const std::vector<std::string>& args);

private:
    TinyKernel& kernel;
};
