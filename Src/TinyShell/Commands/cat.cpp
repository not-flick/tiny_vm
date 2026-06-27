#include "../commands.h"
#include "../filesystem.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

namespace tinyvm
{
    void CatCommand(ShellState& state, const std::vector<std::string>& args)
    {
        if (args.size() < 2)
        {
            std::cout << "Usage: cat <file>\n";
            return;
        }

        fs::path path = ResolvePath(state, args[1]);
        std::error_code error;
        if (!fs::exists(path, error) || !fs::is_regular_file(path, error))
        {
            std::cout << "No such file\n";
            return;
        }

        std::ifstream file(path);
        if (!file)
        {
            std::cout << "Could not read file\n";
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::cout << line << '\n';
        }
    }
}
