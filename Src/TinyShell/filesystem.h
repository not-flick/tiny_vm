#pragma once

#include "shell.h"

#include <filesystem>
#include <string>

namespace tinyvm
{
    std::filesystem::path ResolvePath(const ShellState& state, const std::string& input);
    std::string FormatShellPath(const ShellState& state, const std::filesystem::path& path);
    bool IsInsideRoot(const ShellState& state, const std::filesystem::path& path);
}
