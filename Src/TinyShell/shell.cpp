#include "shell.h"

#include "commands.h"
#include "filesystem.h"
#include "parser.h"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#include <io.h>
#include <windows.h>
#endif

namespace fs = std::filesystem;

namespace tinyvm
{
    namespace
    {
        constexpr const char* ConfigPath = "C:/tiny_vm/system/config.txt";

        void ConfigureTerminal()
        {
#ifdef _WIN32
            SetConsoleOutputCP(CP_UTF8);
            SetConsoleCP(CP_UTF8);
            SetConsoleTitleA("TinyShell");
#endif
        }

#ifdef _WIN32
        fs::path GetExecutableDirectory()
        {
            std::wstring path(MAX_PATH, L'\0');
            const DWORD length = GetModuleFileNameW(nullptr, path.data(), static_cast<DWORD>(path.size()));
            if (length == 0)
            {
                return {};
            }

            path.resize(length);
            return fs::path(path).parent_path();
        }

        void SetWindowIcon(const fs::path& iconPath)
        {
            const std::wstring icon = iconPath.wstring();
            HICON bigIcon = static_cast<HICON>(LoadImageW(
                nullptr,
                icon.c_str(),
                IMAGE_ICON,
                32,
                32,
                LR_LOADFROMFILE));

            HICON smallIcon = static_cast<HICON>(LoadImageW(
                nullptr,
                icon.c_str(),
                IMAGE_ICON,
                16,
                16,
                LR_LOADFROMFILE));

            HWND window = GetConsoleWindow();
            if (window != nullptr)
            {
                if (bigIcon != nullptr)
                {
                    SendMessageW(window, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(bigIcon));
                }

                if (smallIcon != nullptr)
                {
                    SendMessageW(window, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(smallIcon));
                }
            }
        }

        void SetWindowsTerminalTabIcon(const fs::path& iconPath)
        {
            if (std::getenv("WT_SESSION") == nullptr)
            {
                return;
            }

            std::cout << "\x1b]9;9;" << iconPath.generic_string() << "\x07";
            std::cout.flush();
        }

        void ConfigureIcons()
        {
            const fs::path iconPath = GetExecutableDirectory() / "TinyShell.ico";
            std::error_code error;
            if (!fs::exists(iconPath, error))
            {
                return;
            }

            SetWindowIcon(iconPath);
            SetWindowsTerminalTabIcon(iconPath);
        }
#else
        void ConfigureIcons()
        {
        }
#endif

        std::string BuildPrompt(const ShellState& state)
        {
            return state.username + "@" + state.hostname + ":" + FormatShellPath(state, state.cwd) + "$ ";
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

#ifdef _WIN32
        void RedrawInputLine(const std::string& prompt, const std::string& input)
        {
            std::cout << "\r\x1b[2K" << prompt << input;
            std::cout.flush();
        }

        std::string ReadInputLine(const std::string& prompt, const ShellState& state, const CommandMap& commands)
        {
            if (_isatty(_fileno(stdin)) == 0)
            {
                std::string input;
                std::getline(std::cin, input);
                return input;
            }

            std::string input;
            std::size_t historyIndex = state.history.size();

            while (true)
            {
                const int ch = _getch();

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
                        std::cout << "\b \b";
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
                    const int key = _getch();

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
                    std::cout << static_cast<char>(ch);
                }
            }
        }
#else
        std::string ReadInputLine(const std::string&, const ShellState&, const CommandMap&)
        {
            std::string input;
            std::getline(std::cin, input);
            return input;
        }
#endif

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

        std::unordered_map<std::string, std::string> ReadConfig()
        {
            std::unordered_map<std::string, std::string> values;
            std::ifstream file(ConfigPath);

            if (!file.is_open())
            {
                std::cerr << "Failed to open config.txt\n";
                return values;
            }

            std::string line;
            while (std::getline(file, line))
            {
                const std::size_t pos = line.find('=');
                if (pos == std::string::npos)
                {
                    continue;
                }

                values[line.substr(0, pos)] = line.substr(pos + 1);
            }

            return values;
        }
    }

    ShellState LoadConfig()
    {
        ShellState state;
        state.username = "user";
        state.hostname = "tinyvm";
        state.root = fs::path("C:/tiny_vm");
        state.home = state.root / "c" / "users" / state.username;
        state.cwd = state.home;
        state.running = true;

        const auto config = ReadConfig();

        if (auto it = config.find("username"); it != config.end() && !it->second.empty())
        {
            state.username = it->second;
        }

        if (auto it = config.find("hostname"); it != config.end() && !it->second.empty())
        {
            state.hostname = it->second;
        }

        state.root = fs::path("C:/tiny_vm").lexically_normal();
        state.home = (state.root / "c" / "users" / state.username).lexically_normal();
        state.cwd = state.home;

        std::error_code error;
        fs::create_directories(state.home, error);
        if (error)
        {
            std::cerr << "Could not initialize home directory: " << error.message() << '\n';
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
