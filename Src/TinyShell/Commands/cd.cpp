#include "../commands.h"
#include "../filesystem.h"
#include "syscall.h"

#include <iostream>

namespace tinyvm
{
    void CdCommand(ShellState& state, const std::vector<std::string>& args)
    {
        const std::string target = args.size() > 1 ? args[1] : "~";
        FileSystem filesystem(state);

        std::string resolved = filesystem.Resolve(target);

        auto res = Syscall(SyscallID::SET_CWD, {resolved});
        if (!res.success)
        {
            std::cout << "cd: " << (res.error.empty() ? "no such directory" : res.error) << '\n';
            return;
        }

        auto get_res = Syscall(SyscallID::GET_CWD, {});
        if (get_res.success && !get_res.data.empty())
        {
            state.cwd = get_res.data[0];
        }
        else
        {
            state.cwd = resolved;
        }
    }
}
