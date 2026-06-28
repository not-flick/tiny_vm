#include "shell.h"
#include "commands.h"
#include "filesystem.h"
#include "parser.h"
#include "syscall.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace tinyvm
{
    namespace
    {
        void ConfigureTerminal()
        {
            Syscall(SyscallID::CONSOLE_INIT, {"TinyShell"});
        }

        void ConfigureIcons()
        {
            // Fully handled by kernel CONSOLE_INIT system call.
        }

        std::string BuildPrompt(const ShellState& state)
        {
            // ANSI color codes: bright green, bright cyan, bright yellow, reset
            constexpr const char* kGreen = "\x1b[1;32m";
            constexpr const char* kCyan = "\x1b[1;36m";
            constexpr const char* kYellow = "\x1b[1;33m";
            constexpr const char* kReset = "\x1b[0m";

            std::string path = FormatShellPath(state, state.cwd);

            std::string prompt;
            prompt.reserve(64 + state.username.size() + state.hostname.size() + path.size());

            prompt += kGreen;
            prompt += state.username;
            prompt += kReset;
            prompt += "@";
            prompt += kCyan;
            prompt += state.hostname;
            prompt += kReset;
            prompt += ":";
            prompt += kYellow;
            prompt += path;
            prompt += kReset;
            prompt += "$ ";

            return prompt;
        }

        std::vector<std::string> CommandNames(const CommandMap& commands)
        {
            std::vector<std::string> names;
            names.reserve(commands.size());

            for (const auto& [name, command] : commands)
            {
                names.push_back(name);
            }

            std::sort(names.begin(), names.end());
            return names;
        }

        std::string CompleteCommandName(const std::string& input, const CommandMap& commands)
        {
            if (input.find(' ') != std::string::npos || input.find('\t') != std::string::npos)
            {
                return input;
            }

            std::vector<std::string> matches;
            for (const auto& [name, command] : commands)
            {
                if (name.rfind(input, 0) == 0)
                {
                    matches.push_back(name);
                }
            }

            if (matches.size() == 1)
            {
                return matches.front();
            }

            return input;
        }

        std::vector<std::string> ExpandAlias(const ShellState& state, const std::vector<std::string>& tokens)
        {
            if (tokens.empty())
            {
                return tokens;
            }

            const auto alias = state.aliases.find(tokens.front());
            if (alias == state.aliases.end())
            {
                return tokens;
            }

            std::vector<std::string> expanded = Tokenize(alias->second);
            expanded.insert(expanded.end(), tokens.begin() + 1, tokens.end());
            return expanded;
        }

        void RedrawInputLine(const std::string& prompt, const std::string& input)
        {
            std::cout << "\r\x1b[2K" << prompt << input;
            std::cout.flush();
        }

        std::string ReadInputLine(const std::string& prompt, const ShellState& state, const CommandMap& commands)
        {
            auto atty_res = Syscall(SyscallID::CONSOLE_IS_ATTY, {});
            bool is_atty = (atty_res.success && !atty_res.data.empty() && atty_res.data[0] == "true");

            if (!is_atty)
            {
                auto read_res = Syscall(SyscallID::CONSOLE_READ_LINE, {});
                return (read_res.success && !read_res.data.empty()) ? read_res.data[0] : "";
            }

            std::string input;
            std::size_t historyIndex = state.history.size();

            while (true)
            {
                auto char_res = Syscall(SyscallID::CONSOLE_READ_CHAR, {});
                if (!char_res.success || char_res.data.empty())
                {
                    break;
                }
                const int ch = std::stoi(char_res.data[0]);

                if (ch == '\r')
                {
                    std::cout << '\n';
                    return input;
                }

                if (ch == '\b')
                {
                    if (!input.empty())
                    {
                        input.pop_back();
                        RedrawInputLine(prompt, input);
                    }
                    continue;
                }

                if (ch == '\t')
                {
                    const std::string completed = CompleteCommandName(input, commands);
                    if (completed != input)
                    {
                        input = completed;
                        RedrawInputLine(prompt, input);
                    }
                    continue;
                }

                if (ch == 0 || ch == 224)
                {
                    auto key_res = Syscall(SyscallID::CONSOLE_READ_CHAR, {});
                    if (!key_res.success || key_res.data.empty())
                    {
                        continue;
                    }
                    const int key = std::stoi(key_res.data[0]);

                    if (key == 72 && historyIndex > 0)
                    {
                        --historyIndex;
                        input = state.history[historyIndex];
                        RedrawInputLine(prompt, input);
                    }
                    else if (key == 80 && historyIndex < state.history.size())
                    {
                        ++historyIndex;
                        input = historyIndex < state.history.size() ? state.history[historyIndex] : "";
                        RedrawInputLine(prompt, input);
                    }

                    continue;
                }

                if (ch >= 32 && ch <= 126)
                {
                    input.push_back(static_cast<char>(ch));
                    RedrawInputLine(prompt, input);
                }
            }
            return input;
        }

        void PrintBanner()
        {
            std::cout << R"(

========================================
████████╗██╗███╗   ██╗██╗   ██╗██╗   ██╗███╗   ███╗
╚══██╔══╝██║████╗  ██║╚██╗ ██╔╝██║   ██║████╗ ████║
   ██║   ██║██╔██╗ ██║ ╚████╔╝ ██║   ██║██╔████╔██║
   ██║   ██║██║╚██╗██║  ╚██╔╝  ╚██╗ ██╔╝██║╚██╔╝██║
   ██║   ██║██║ ╚████║   ██║    ╚████╔╝ ██║ ╚═╝ ██║
   ╚═╝   ╚═╝╚═╝  ╚═══╝   ╚═╝     ╚═══╝  ╚═╝     ╚═╝

               TinyVM Shell v0.1

========================================

)";
        }
    }

    ShellState LoadConfig()
    {
        ShellState state;
        state.username = "user";
        state.hostname = "tinyvm";
        state.cwd = "/c/users/user";
        state.running = true;

        auto user_res = Syscall(SyscallID::GET_USERNAME, {});
        if (user_res.success && !user_res.data.empty())
        {
            state.username = user_res.data[0];
        }

        auto host_res = Syscall(SyscallID::GET_HOSTNAME, {});
        if (host_res.success && !host_res.data.empty())
        {
            state.hostname = host_res.data[0];
        }

        auto cwd_res = Syscall(SyscallID::GET_CWD, {});
        if (cwd_res.success && !cwd_res.data.empty())
        {
            state.cwd = cwd_res.data[0];
        }

        return state;
    }

    void RunShell(ShellState& state)
    {
        ConfigureTerminal();
        ConfigureIcons();
        PrintBanner();

        const CommandMap commands = CreateCommandRegistry();

        while (state.running)
        {
            const std::string prompt = BuildPrompt(state);
            std::cout << prompt;

            std::string input = ReadInputLine(prompt, state, commands);
            if (!std::cin && input.empty())
            {
                break;
            }

            std::vector<std::string> tokens = Tokenize(input);
            if (tokens.empty())
            {
                continue;
            }

            state.history.push_back(input);
            tokens = ExpandAlias(state, tokens);
            if (tokens.empty())
            {
                continue;
            }

            const auto command = commands.find(tokens.front());
            if (command == commands.end())
            {
                std::cout << "tinyvm: command not found: " << tokens.front() << '\n';
                continue;
            }

            command->second.handler(state, tokens);
        }

        std::cout << "Goodbye!\n";
    }
}
