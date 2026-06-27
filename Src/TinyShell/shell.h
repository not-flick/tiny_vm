#pragma once

#include <filesystem>
#include <string>

namespace tinyvm
{
    struct ShellState
    {
        std::string username;
        std::string hostname;

        std::filesystem::path root;
        std::filesystem::path home;
        std::filesystem::path cwd;

        bool running;
    };

    ShellState LoadConfig();
    void RunShell(ShellState& state);
}
