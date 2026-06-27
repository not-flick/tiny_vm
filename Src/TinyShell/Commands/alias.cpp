#include "../commands.h"

#include <iostream>

namespace tinyvm
{
    void AliasCommand(ShellState& state, const std::vector<std::string>& args)
    {
        if (args.size() == 1)
        {
            for (const auto& [name, value] : state.aliases)
            {
                std::cout << "alias " << name << "=\"" << value << "\"\n";
            }
            return;
        }

        for (std::size_t index = 1; index < args.size(); ++index)
        {
            const std::string& assignment = args[index];
            const std::size_t equals = assignment.find('=');
            if (equals == std::string::npos || equals == 0)
            {
                std::cout << "alias: invalid alias\n";
                continue;
            }

            state.aliases[assignment.substr(0, equals)] = assignment.substr(equals + 1);
        }
    }
}
