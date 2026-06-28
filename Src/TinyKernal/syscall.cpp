#include "syscall.h"
#include "kernal.h"
#include "filesystems.h"

#include <cstdlib>
#include <exception>

#ifdef CreateFile
#undef CreateFile
#endif

#ifdef DeleteFile
#undef DeleteFile
#endif

#ifdef CreateDirectory
#undef CreateDirectory
#endif

SyscallDispatcher::SyscallDispatcher(TinyKernel& kernel)
    : kernel(kernel)
{
}

bool SyscallDispatcher::Handle(
    SyscallID id,
    const std::vector<std::string>& args)
{
    switch (id)
    {
        case SyscallID::PRINT:
        {
            if (!args.empty())
                kernel.console.Print(args[0]);
            return true;
        }

        case SyscallID::CLEAR:
        {
            kernel.console.Clear();
            return true;
        }

        case SyscallID::CREATE_FILE:
        {
            if (!args.empty())
                return kernel.filesystem.CreateFile(args[0]);

            return false;
        }

        case SyscallID::DELETE_FILE:
        {
            if (!args.empty())
                return kernel.filesystem.DeleteFile(args[0]);

            return false;
        }

        case SyscallID::CREATE_DIRECTORY:
        {
            if (!args.empty())
                return kernel.filesystem.CreateDirectory(args[0]);

            return false;
        }

        case SyscallID::DELETE_DIRECTORY:
        {
            if (!args.empty())
                return kernel.filesystem.DeleteDirectory(args[0]);

            return false;
        }

        case SyscallID::FILE_EXISTS:
        {
            if (!args.empty())
                return kernel.filesystem.Exists(args[0]);

            return false;
        }

        case SyscallID::RUN_PROCESS:
        {
            if (!args.empty())
            {
                kernel.process.Launch(args[0]);
                return true;
            }

            return false;
        }

        case SyscallID::EXIT:
        {
            int code = 0;

            if (!args.empty())
            {
                try
                {
                    code = std::stoi(args[0]);
                }
                catch (const std::exception&)
                {
                    return false;
                }
            }

            kernel.process.Exit(code);
            return true;
        }

        default:
            return false;
    }
}
