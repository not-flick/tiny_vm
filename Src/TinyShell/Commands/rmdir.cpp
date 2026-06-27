#include "../commands.h"
#include "../filesystem.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace tinyvm
{
    void RmdirCommand(ShellState& state, const std::vector<std::string>& args)
    {
        if (args.size() < 2)
        {
            std::cout << "Usage: rmdir <dir>\n";
            return;
        }

        fs::path path = ResolvePath(state, args[1]);
        std::error_code error;
        if (!fs::exists(path, error) || !fs::is_directory(path, error))
        {
            std::cout << "No such directory\n";
            return;
        }

        if (!fs::remove(path, error))
        {
            std::cout << "Could not remove directory";
            if (error)
            {
                std::cout << ": " << error.message();
            }
            std::cout << '\n';
        }
    }
}
