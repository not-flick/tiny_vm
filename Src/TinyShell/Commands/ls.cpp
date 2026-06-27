#include "../commands.h"
#include "../filesystem.h"

#include <iostream>
#include <vector>

namespace tinyvm
{
    void LsCommand(ShellState& state, const std::vector<std::string>& args)
    {
        const std::string target = args.size() > 1 ? args[1] : "";
        FileSystem filesystem(state);

        if (!filesystem.IsDirectory(target))
        {
            std::cout << "ls: no such directory\n";
            return;
        }

        const std::vector<std::string> entries = filesystem.List(target);
        if (!filesystem.LastError().empty())
        {
            std::cout << "ls: " << filesystem.LastError() << '\n';
            return;
        }

        for (const std::string& entry : entries)
        {
            std::cout << entry << '\n';
        }
    }
}
