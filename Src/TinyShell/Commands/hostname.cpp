#include "../commands.h"

#include <iostream>

namespace tinyvm
{
    void HostnameCommand(ShellState& state, const std::vector<std::string>&)
    {
        std::cout << state.hostname << '\n';
    }
}
