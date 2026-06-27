#ifndef PROJECT_ROOT
#define PROJECT_ROOT "."
#endif

#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void create_directory(const std::string& username)
{
    // PROJECT_ROOT is defined by CMake
    fs::path disk = fs::path(PROJECT_ROOT) / "Disk";

    fs::create_directories(disk / "bin");
    fs::create_directories(disk / "boot");
    fs::create_directories(disk / "dev");
    fs::create_directories(disk / "etc");

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

    return 0;
}