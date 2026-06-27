#include "parser.h"

#include <cctype>

namespace tinyvm
{
    std::vector<std::string> Tokenize(const std::string& input)
    {
        std::vector<std::string> tokens;
        std::string current;
        bool inQuotes = false;
        bool tokenStarted = false;

        for (std::size_t index = 0; index < input.size(); ++index)
        {
            const char ch = input[index];

            if (ch == '\\' && index + 1 < input.size())
            {
                const char next = input[index + 1];
                if (next == '"' || next == '\\')
                {
                    current.push_back(next);
                    tokenStarted = true;
                    ++index;
                    continue;
                }
            }

            if (ch == '"')
            {
                if (inQuotes && index + 1 < input.size() && input[index + 1] == '"')
                {
                    current.push_back('"');
                    tokenStarted = true;
                    ++index;
                    continue;
                }

                inQuotes = !inQuotes;
                tokenStarted = true;
                continue;
            }

            if (!inQuotes && std::isspace(static_cast<unsigned char>(ch)) != 0)
            {
                if (tokenStarted)
                {
                    tokens.push_back(current);
                    current.clear();
                    tokenStarted = false;
                }
                continue;
            }

            current.push_back(ch);
            tokenStarted = true;
        }

        if (tokenStarted)
        {
            tokens.push_back(current);
        }

        return tokens;
    }
}
