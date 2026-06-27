#include "../commands.h"

#include <algorithm>
#include <iostream>
#include <vector>

namespace tinyvm
{
    void HelpCommand(ShellState&, const std::vector<std::string>&)
    {
        CommandMap commands = CreateCommandRegistry();
        std::vector<Command> sorted;
        sorted.reserve(commands.size());

        for (const auto& [name, command] : commands)
        {
            sorted.push_back(command);
        }

        std::sort(sorted.begin(), sorted.end(), [](const Command& left, const Command& right) {
            return left.name < right.name;
        });

        for (const Command& command : sorted)
        {
            std::cout << command.name << " - " << command.description << '\n';
        }
    }
}
