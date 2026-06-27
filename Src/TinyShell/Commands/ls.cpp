#include "../commands.h"
#include "../filesystem.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

namespace tinyvm
{
    void LsCommand(ShellState& state, const std::vector<std::string>& args)
    {
        fs::path path = args.size() > 1 ? ResolvePath(state, args[1]) : state.cwd;

        std::error_code error;
        if (!fs::exists(path, error) || !fs::is_directory(path, error))
        {
            std::cout << "No such directory\n";
            return;
        }

        std::vector<fs::directory_entry> entries;
        for (const auto& entry : fs::directory_iterator(path, error))
        {
            if (error)
            {
                std::cout << "Unable to list directory: " << error.message() << '\n';
                return;
            }

            entries.push_back(entry);
        }

        std::sort(entries.begin(), entries.end(), [](const fs::directory_entry& left, const fs::directory_entry& right) {
            return left.path().filename().generic_string() < right.path().filename().generic_string();
        });

        for (const auto& entry : entries)
        {
            std::cout << entry.path().filename().generic_string();
            if (entry.is_directory(error))
            {
                std::cout << "/";
            }
            std::cout << '\n';
        }
    }
}
