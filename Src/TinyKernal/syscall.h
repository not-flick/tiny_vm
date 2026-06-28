#pragma once

#include <string>
#include <vector>

class TinyKernel;

enum class SyscallID
{
    PRINT,
    CLEAR,

    CONSOLE_INIT,
    CONSOLE_IS_ATTY,
    CONSOLE_READ_CHAR,
    CONSOLE_READ_LINE,

    CREATE_FILE,
    DELETE_FILE,

    CREATE_DIRECTORY,
    DELETE_DIRECTORY,

    FILE_EXISTS,
    IS_DIRECTORY,
    IS_FILE,

    READ_FILE,
    WRITE_FILE,

    LIST_DIRECTORY,
    COPY_FILE,
    MOVE_FILE,

    GET_CWD,
    SET_CWD,
    GET_USERNAME,
    GET_HOSTNAME,

    RUN_PROCESS,
    KILL_PROCESS,

    ALLOCATE_MEMORY,
    FREE_MEMORY,

    EXIT
};

struct SyscallResult
{
    bool success{false};
    std::string error;
    std::vector<std::string> data;
};

class SyscallDispatcher
{
public:
    explicit SyscallDispatcher(TinyKernel& kernel);

    SyscallResult Handle(
        SyscallID id,
        const std::vector<std::string>& args);

private:
    TinyKernel& kernel;
};

SyscallResult Syscall(SyscallID id, const std::vector<std::string>& args);
