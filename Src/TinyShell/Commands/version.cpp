#include "../commands.h"

#include <iostream>

namespace tinyvm
{
    void VersionCommand(ShellState&, const std::vector<std::string>&)
    {
        std::cout << "TinyVM Shell v0.1\n";
    }
}
