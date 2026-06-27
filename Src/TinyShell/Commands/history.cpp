#include "../commands.h"

#include <iostream>

namespace tinyvm
{
    void HistoryCommand(ShellState& state, const std::vector<std::string>&)
    {
        for (std::size_t index = 0; index < state.history.size(); ++index)
        {
            std::cout << index + 1 << "  " << state.history[index] << '\n';
        }
    }
}
