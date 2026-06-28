#include "../commands.h"
#include "../filesystem.h"

#include <iostream>

namespace tinyvm
{
    void MkdirCommand(ShellState& state, const std::vector<std::string>& args)
    {
        if (args.size() < 2)
        {
            std::cout << "mkdir: missing operand\n";
            return;
        }

        FileSystem filesystem(state);
        if (!filesystem.MkDir(args[1]))
        {
            std::cout << "mkdir: " << (filesystem.LastError().empty() ? "could not create directory" : filesystem.LastError()) << '\n';
        }
    }
}
