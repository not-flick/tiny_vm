#include "../commands.h"
#include "../filesystem.h"

#include <iostream>

namespace tinyvm
{
    void TouchCommand(ShellState& state, const std::vector<std::string>& args)
    {
        if (args.size() < 2)
        {
            std::cout << "touch: missing operand\n";
            return;
        }

        FileSystem filesystem(state);
        if (filesystem.IsDirectory(args[1]))
        {
            std::cout << "touch: cannot touch directory\n";
            return;
        }

        if (!filesystem.Touch(args[1]))
        {
            std::cout << "touch: " << (filesystem.LastError().empty() ? "could not create file" : filesystem.LastError()) << '\n';
        }
    }
}
