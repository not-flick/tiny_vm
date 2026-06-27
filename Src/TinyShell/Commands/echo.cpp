#include "../commands.h"

#include <iostream>

namespace tinyvm
{
    void EchoCommand(ShellState&, const std::vector<std::string>& args)
    {
        for (std::size_t index = 1; index < args.size(); ++index)
        {
            if (index > 1)
            {
                std::cout << ' ';
            }
            std::cout << args[index];
        }
        std::cout << '\n';
    }
}
