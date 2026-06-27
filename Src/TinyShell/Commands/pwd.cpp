#include "../commands.h"
#include "../filesystem.h"

#include <iostream>

namespace tinyvm
{
    void PwdCommand(ShellState& state, const std::vector<std::string>&)
    {
        std::cout << FormatShellPath(state, state.cwd) << '\n';
    }
}
