#include "../commands.h"

#include <iostream>

namespace tinyvm
{
    void WhoamiCommand(ShellState& state, const std::vector<std::string>&)
    {
        std::cout << state.username << '\n';
    }
}
