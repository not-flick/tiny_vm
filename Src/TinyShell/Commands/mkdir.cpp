#include "../commands.h"
#include "../filesystem.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace tinyvm
{
    void MkdirCommand(ShellState& state, const std::vector<std::string>& args)
    {
        if (args.size() < 2)
        {
            std::cout << "Usage: mkdir <dir>\n";
            return;
        }

        fs::path path = ResolvePath(state, args[1]);
        std::error_code error;
        if (!fs::create_directory(path, error))
        {
            if (error)
            {
                std::cout << "Could not create directory: " << error.message() << '\n';
            }
            else
            {
                std::cout << "Could not create directory\n";
            }
        }
    }
}
