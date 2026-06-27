#include "filesystem.h"

#include <algorithm>

namespace fs = std::filesystem;

namespace tinyvm
{
    namespace
    {
        std::string ToForwardSlashes(std::string text)
        {
            std::replace(text.begin(), text.end(), '\\', '/');
            return text;
        }

        bool StartsWithPath(const fs::path& path, const fs::path& prefix)
        {
            auto pathIt = path.begin();
            auto prefixIt = prefix.begin();

            for (; prefixIt != prefix.end(); ++prefixIt, ++pathIt)
            {
                if (pathIt == path.end() || *pathIt != *prefixIt)
                {
                    return false;
                }
            }

            return true;
        }
    }

    fs::path ResolvePath(const ShellState& state, const std::string& input)
    {
        if (input.empty())
        {
            return state.cwd.lexically_normal();
        }

        std::string value = ToForwardSlashes(input);
        fs::path resolved;

        if (value == "~")
        {
            resolved = state.home;
        }
        else if (value.rfind("~/", 0) == 0)
        {
            resolved = state.home / value.substr(2);
        }
        else if (value.front() == '/')
        {
            resolved = state.root / value.substr(1);
        }
        else
        {
            resolved = state.cwd / value;
        }

        resolved = resolved.lexically_normal();

        if (!IsInsideRoot(state, resolved))
        {
            return state.root;
        }

        return resolved;
    }

    std::string FormatShellPath(const ShellState& state, const fs::path& path)
    {
        fs::path normalized = path.lexically_normal();
        fs::path home = state.home.lexically_normal();
        fs::path root = state.root.lexically_normal();

        if (normalized == home)
        {
            return "~";
        }

        if (StartsWithPath(normalized, home))
        {
            fs::path relative = normalized.lexically_relative(home);
            return "~/" + ToForwardSlashes(relative.generic_string());
        }

        if (normalized == root)
        {
            return "/";
        }

        fs::path relative = normalized.lexically_relative(root);
        std::string display = ToForwardSlashes(relative.generic_string());
        if (display.empty() || display == ".")
        {
            return "/";
        }

        return "/" + display;
    }

    bool IsInsideRoot(const ShellState& state, const fs::path& path)
    {
        fs::path normalized = path.lexically_normal();
        fs::path root = state.root.lexically_normal();
        return normalized == root || StartsWithPath(normalized, root);
    }
}
