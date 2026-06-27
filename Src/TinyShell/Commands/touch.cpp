#include "../commands.h"
#include "../filesystem.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace tinyvm
{
    void TouchCommand(ShellState& state, const std::vector<std::string>& args)
    {
        if (args.size() < 2)
        {
            std::cout << "Usage: touch <file>\n";
            return;
        }

        fs::path path = ResolvePath(state, args[1]);
        std::error_code error;
        if (fs::exists(path, error) && fs::is_directory(path, error))
        {
            std::cout << "Cannot touch directory\n";
            return;
        }

        std::ofstream file(path, std::ios::app);
        if (!file)
        {
            std::cout << "Could not create file\n";
        }
    }
}
