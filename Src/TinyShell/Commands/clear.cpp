#include "../commands.h"
#include "syscall.h"

namespace tinyvm
{
    void ClearCommand(ShellState&, const std::vector<std::string>&)
    {
        Syscall(SyscallID::CLEAR, {});
    }
}
