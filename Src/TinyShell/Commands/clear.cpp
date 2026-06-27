#include "../commands.h"

#include <iostream>

namespace tinyvm
{
    void ClearCommand(ShellState&, const std::vector<std::string>&)
    {
        std::cout << "\x1B[2J\x1B[H";
    }
}
