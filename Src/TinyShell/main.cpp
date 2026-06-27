#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

struct Command
{
    std::string name;
    std::string description;

    void (*handler)(const std::vector<std::string>&);
};

struct ShellState
{
    std::string username;
    std::string hostname;
    std::string home;
    std::string cwd;   
    std::string version;

    bool running = true;
};

ShellState LoadConfig()
{
    ShellState state;

    std::ifstream file("C:\\tiny_vm\\system\\config.txt");

    if (!file.is_open())
    {
        std::cerr << "Failed to open config.txt\n";
        return state;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::size_t pos = line.find('=');

        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        if (key == "username")
            state.username = value;

        else if (key == "hostname")
            state.hostname = value;

        else if (key == "home")
            state.home = value;

        else if (key == "cwd")
            state.cwd = value;

        else if (key == "version")
            state.version = value;
    }

    return state;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    SetConsoleTitleA("TinyShell");

    ShellState state = LoadConfig();
    state.home = "c:/users/" + state.username;

    std::string banner = R"(

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

    std::cout << banner;

    while (state.running)
    {
        std::string displayPath = state.cwd;

        if (displayPath.rfind(state.home, 0) == 0)
        {
            displayPath.replace(0, state.home.length(), "~");
        }

        std::cout
            << state.username
            << "@"
            << state.hostname
            << ":"
            << displayPath
            << "$ ";

        std::string input;
        std::getline(std::cin, input);

        if (input == "exit")
        {
            state.running = false;
            continue;
        }

        if (input.empty())
            continue;

        std::cout << "Unknown command: " << input << '\n';
    }

    std::cout << "Goodbye!\n";

    return 0;
}