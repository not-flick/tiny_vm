#include "../commands.h"

#include <iostream>

namespace tinyvm
{
    void SetCommand(ShellState& state, const std::vector<std::string>& args)
    {
        if (args.size() == 1)
        {
            for (const auto& [name, value] : state.environment)
            {
                std::cout << name << "=" << value << '\n';
            }
            return;
        }

        for (std::size_t index = 1; index < args.size(); ++index)
        {
            const std::string& assignment = args[index];
            const std::size_t equals = assignment.find('=');
            if (equals == std::string::npos || equals == 0)
            {
                std::cout << "set: invalid assignment\n";
                continue;
            }

            state.environment[assignment.substr(0, equals)] = assignment.substr(equals + 1);
        }
    }
}
