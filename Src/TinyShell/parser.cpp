#include "parser.h"

#include <cctype>

namespace tinyvm
{
    std::vector<std::string> Tokenize(const std::string& input)
    {
        std::vector<std::string> tokens;
        std::string current;
        bool inQuotes = false;

        for (char ch : input)
        {
            if (ch == '"')
            {
                inQuotes = !inQuotes;
                continue;
            }

            if (!inQuotes && std::isspace(static_cast<unsigned char>(ch)) != 0)
            {
                if (!current.empty())
                {
                    tokens.push_back(current);
                    current.clear();
                }
                continue;
            }

            current.push_back(ch);
        }

        if (!current.empty())
        {
            tokens.push_back(current);
        }

        return tokens;
    }
}
