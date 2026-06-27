#include "../commands.h"

#include <iostream>

namespace tinyvm
{
    void UnsetCommand(ShellState& state, const std::vector<std::string>& args)
    {
        if (args.size() < 2)
        {
            std::cout << "unset: missing operand\n";
            return;
        }

        for (std::size_t index = 1; index < args.size(); ++index)
        {
            state.environment.erase(args[index]);
        }
    }
}
