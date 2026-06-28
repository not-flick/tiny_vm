#include "../commands.h"
#include "../filesystem.h"

#include <iostream>

namespace tinyvm
{
    void RmCommand(ShellState& state, const std::vector<std::string>& args)
    {
        if (args.size() < 2)
        {
            std::cout << "rm: missing operand\n";
            return;
        }

        FileSystem filesystem(state);
        if (!filesystem.IsFile(args[1]))
        {
            std::cout << "rm: file not found\n";
            return;
        }

        if (!filesystem.RmFile(args[1]))
        {
            std::cout << "rm: " << (filesystem.LastError().empty() ? "could not remove file" : filesystem.LastError()) << '\n';
        }
    }
}
