#include "../commands.h"
#include "../filesystem.h"

#include <iostream>
#include <string>

namespace tinyvm
{
    void CatCommand(ShellState& state, const std::vector<std::string>& args)
    {
        if (args.size() < 2)
        {
            std::cout << "cat: missing operand\n";
            return;
        }

        FileSystem filesystem(state);
        if (!filesystem.IsFile(args[1]))
        {
            std::cout << "cat: file not found\n";
            return;
        }

        const std::string contents = filesystem.ReadFile(args[1]);
        if (!filesystem.LastError().empty())
        {
            std::cout << "cat: " << filesystem.LastError() << '\n';
            return;
        }

        std::cout << contents;
        if (!contents.empty() && contents.back() != '\n')
            std::cout << '\n';
    }
}
