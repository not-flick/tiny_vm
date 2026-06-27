#include "../commands.h"
#include "../filesystem.h"

#include <iostream>

namespace tinyvm
{
    void RmdirCommand(ShellState& state, const std::vector<std::string>& args)
    {
        if (args.size() < 2)
        {
            std::cout << "rmdir: missing operand\n";
            return;
        }

        FileSystem filesystem(state);
        if (!filesystem.IsDirectory(args[1]))
        {
            std::cout << "rmdir: no such directory\n";
            return;
        }

        if (!filesystem.RemoveDirectory(args[1]))
        {
            std::cout << "rmdir: " << (filesystem.LastError().empty() ? "could not remove directory" : filesystem.LastError()) << '\n';
        }
    }
}
