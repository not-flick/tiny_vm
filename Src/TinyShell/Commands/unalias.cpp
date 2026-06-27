#include "../commands.h"

#include <iostream>

namespace tinyvm
{
    void UnaliasCommand(ShellState& state, const std::vector<std::string>& args)
    {
        if (args.size() < 2)
        {
            std::cout << "unalias: missing operand\n";
            return;
        }

        for (std::size_t index = 1; index < args.size(); ++index)
        {
            if (state.aliases.erase(args[index]) == 0)
            {
                std::cout << "unalias: not found: " << args[index] << '\n';
            }
        }
    }
}
