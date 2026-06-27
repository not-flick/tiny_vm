#pragma once

#include <filesystem>
#include <unordered_map>
#include <string>
#include <vector>

namespace tinyvm
{
    struct ShellState
    {
        std::string username;
        std::string hostname;

        std::filesystem::path root;
        std::filesystem::path home;
        std::filesystem::path cwd;

        std::vector<std::string> history;
        std::unordered_map<std::string, std::string> aliases;
        std::unordered_map<std::string, std::string> environment;

        bool running;
    };

    ShellState LoadConfig();
    void RunShell(ShellState& state);
}
