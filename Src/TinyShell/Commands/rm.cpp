#include "../commands.h"
#include "../filesystem.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace tinyvm
{
    void RmCommand(ShellState& state, const std::vector<std::string>& args)
    {
        if (args.size() < 2)
        {
            std::cout << "Usage: rm <file>\n";
            return;
        }

        fs::path path = ResolvePath(state, args[1]);
        std::error_code error;
        if (!fs::exists(path, error) || !fs::is_regular_file(path, error))
        {
            std::cout << "No such file\n";
            return;
        }

        if (!fs::remove(path, error))
        {
            std::cout << "Could not remove file";
            if (error)
            {
                std::cout << ": " << error.message();
            }
            std::cout << '\n';
        }
    }
}
