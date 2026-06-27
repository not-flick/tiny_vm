#pragma once

#include "shell.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace tinyvm
{
    struct Command
    {
        std::string name;
        std::string description;

        void (*handler)(ShellState&, const std::vector<std::string>&);
    };

    using CommandMap = std::unordered_map<std::string, Command>;

    CommandMap CreateCommandRegistry();

    void HelpCommand(ShellState& state, const std::vector<std::string>& args);
    void ClearCommand(ShellState& state, const std::vector<std::string>& args);
    void ExitCommand(ShellState& state, const std::vector<std::string>& args);
    void PwdCommand(ShellState& state, const std::vector<std::string>& args);
    void CdCommand(ShellState& state, const std::vector<std::string>& args);
    void LsCommand(ShellState& state, const std::vector<std::string>& args);
    void MkdirCommand(ShellState& state, const std::vector<std::string>& args);
    void TouchCommand(ShellState& state, const std::vector<std::string>& args);
    void RmCommand(ShellState& state, const std::vector<std::string>& args);
    void RmdirCommand(ShellState& state, const std::vector<std::string>& args);
    void CatCommand(ShellState& state, const std::vector<std::string>& args);
    void EchoCommand(ShellState& state, const std::vector<std::string>& args);
    void WhoamiCommand(ShellState& state, const std::vector<std::string>& args);
    void HostnameCommand(ShellState& state, const std::vector<std::string>& args);
    void VersionCommand(ShellState& state, const std::vector<std::string>& args);
    void HistoryCommand(ShellState& state, const std::vector<std::string>& args);
    void AliasCommand(ShellState& state, const std::vector<std::string>& args);
    void UnaliasCommand(ShellState& state, const std::vector<std::string>& args);
    void SetCommand(ShellState& state, const std::vector<std::string>& args);
    void UnsetCommand(ShellState& state, const std::vector<std::string>& args);
}
