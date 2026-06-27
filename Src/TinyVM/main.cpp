#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <windows.h>

namespace fs = std::filesystem;

void create_directory(const std::string& username)
{
    // PROJECT_ROOT is defined by CMake
    fs::path disk = "C:/tiny_vm";

    fs::create_directories(disk / "bin");
    fs::create_directories(disk / "boot");
    fs::create_directories(disk / "dev");
    fs::create_directories(disk / "etc");

    fs::create_directories(disk/ "shell");
    fs::create_directories(disk / "system");

    fs::create_directories(disk / "home");
    fs::create_directories(disk / "home" / username);

    fs::create_directories(disk / "lib");
    fs::create_directories(disk / "proc");
    fs::create_directories(disk / "sys");
    fs::create_directories(disk / "tmp");

    fs::create_directories(disk / "usr");
    fs::create_directories(disk / "usr" / "bin");
    fs::create_directories(disk / "usr" / "include");
    fs::create_directories(disk / "usr" / "lib");

    fs::create_directories(disk / "var");
    fs::create_directories(disk / "var" / "log");
    fs::create_directories(disk / "var" / "tmp");

    // Your C drive inside the VM
    fs::create_directories(disk / "c");
    fs::create_directories(disk / "c" / "users");
    fs::create_directories(disk / "c" / "users" / username);

    std::cout << "=====================================\n";
    std::cout << " TinyVM Virtual Disk Initialized\n";
    std::cout << "=====================================\n\n";

    std::cout << "Disk Location : " << disk << '\n';
    std::cout << "User          : " << username << '\n';
    std::cout << "Status        : Ready\n";
}


int main()
{
    std::string username;

    std::cout << "Enter username: ";
    std::getline(std::cin, username);

    create_directory(username);

    std::ofstream config("C:/tiny_vm/system/config.txt");

    config << "username=" << username << '\n';
    config << "hostname=tinyvm" << '\n';
    config << "version=0.1" << '\n';
    config << "cwd=c:/users/" << username << '\n';

    config.close();

    STARTUPINFOA si{};
    PROCESS_INFORMATION pi{};

    si.cb = sizeof(si);


    CreateProcessA(
        "C:\\Users\\viraj\\tiny_vm\\Build\\TinyShell.exe",
        nullptr,
        nullptr,
        nullptr,
        FALSE,
        CREATE_NEW_CONSOLE,
        nullptr,
        "C:\\tiny_vm",
        &si,
        &pi
    );


    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}