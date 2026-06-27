#include "shell.h"

#include "commands.h"
#include "filesystem.h"
#include "parser.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unordered_map>

#ifdef _WIN32
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
            std::cout << state.username
                      << "@"
                      << state.hostname
                      << ":"
                      << FormatShellPath(state, state.cwd)
                      << "$ ";

            std::string input;
            if (!std::getline(std::cin, input))
            {
                break;
            }

            std::vector<std::string> tokens = Tokenize(input);
            if (tokens.empty())
            {
                continue;
            }

            const auto command = commands.find(tokens.front());
            if (command == commands.end())
            {
                std::cout << "Command not found: " << tokens.front() << '\n';
                continue;
            }

            command->second.handler(state, tokens);
        }

        std::cout << "Goodbye!\n";
    }
}
