#include "../commands.h"
#include "../filesystem.h"

#include <iostream>

namespace tinyvm
{
    void CdCommand(ShellState& state, const std::vector<std::string>& args)
    {
        const std::string target = args.size() > 1 ? args[1] : "~";
        FileSystem filesystem(state);

        if (!filesystem.Exists(target) || !filesystem.IsDirectory(target))
        {
            std::cout << "cd: no such directory\n";
            return;
        }

        state.cwd = filesystem.Resolve(target);
    }
}
