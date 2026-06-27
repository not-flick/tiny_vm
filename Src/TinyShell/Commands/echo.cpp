#include "../commands.h"

#include <cctype>
#include <iostream>

namespace tinyvm
{
    namespace
    {
        std::string ExpandEnvironmentVariables(const ShellState& state, const std::string& text)
        {
            std::string expanded;

            for (std::size_t index = 0; index < text.size(); ++index)
            {
                if (text[index] != '$')
                {
                    expanded.push_back(text[index]);
                    continue;
                }

                std::size_t nameStart = index + 1;
                std::size_t nameEnd = nameStart;
                while (nameEnd < text.size() && (std::isalnum(static_cast<unsigned char>(text[nameEnd])) != 0 || text[nameEnd] == '_'))
                {
                    ++nameEnd;
                }

                if (nameEnd == nameStart)
                {
                    expanded.push_back('$');
                    continue;
                }

                const std::string name = text.substr(nameStart, nameEnd - nameStart);
                if (auto variable = state.environment.find(name); variable != state.environment.end())
                {
                    expanded += variable->second;
                }

                index = nameEnd - 1;
            }

            return expanded;
        }
    }

    void EchoCommand(ShellState& state, const std::vector<std::string>& args)
    {
        for (std::size_t index = 1; index < args.size(); ++index)
        {
            if (index > 1)
            {
                std::cout << ' ';
            }
            std::cout << ExpandEnvironmentVariables(state, args[index]);
        }
        std::cout << '\n';
    }
}
