#include "../commands.h"
#include "../filesystem.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace tinyvm
{
    void CdCommand(ShellState& state, const std::vector<std::string>& args)
    {
        const std::string target = args.size() > 1 ? args[1] : "~";
        fs::path path = ResolvePath(state, target);

        std::error_code error;
        if (!fs::exists(path, error) || !fs::is_directory(path, error))
        {
            std::cout << "No such directory\n";
            return;
        }

        state.cwd = path.lexically_normal();
    }
}
