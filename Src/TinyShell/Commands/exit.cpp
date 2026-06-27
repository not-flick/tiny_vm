#include "../commands.h"

namespace tinyvm
{
    void ExitCommand(ShellState& state, const std::vector<std::string>&)
    {
        state.running = false;
    }
}
