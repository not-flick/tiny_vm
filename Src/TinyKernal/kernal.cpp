#include "kernal.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

extern TinyKernel* g_Kernel;

void TinyKernel::Boot()
{
    g_Kernel = this;

    std::string username = "user";
    std::string hostname = "tinyvm";
    
    std::ifstream file("C:/tiny_vm/system/config.txt");
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            size_t pos = line.find('=');
            if (pos != std::string::npos)
            {
                std::string key = line.substr(0, pos);
                std::string val = line.substr(pos + 1);
                if (key == "username" && !val.empty()) username = val;
                if (key == "hostname" && !val.empty()) hostname = val;
            }
        }
        file.close();
    }
    
    filesystem.Initialize(username, hostname);

    // Allocate 256 MB of VM RAM for the tiny VM
    const std::size_t kRamBytes = 256ULL * 1024 * 1024;
    ramBase = memory.Allocate(kRamBytes);

    console.Clear();
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    console.Print("\x1B[1m=====================================\n");
    console.Print("\x1B[1m TinyKernal Boot\n");
    console.Print("=====================================\n\n");
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    console.Print("\x1B[1;32m[  OK  ]\x1B[0m Memory page size : ");
    std::cout << memory.PageSize() << " bytes\n";
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    console.Print("\x1B[1;32m[  OK  ]\x1B[0m Total RAM         : ");
    if (ramBase)
    {
        std::cout << kRamBytes << " bytes\n";
    }
    else
    {
        console.Print("allocation failed\n");
    }
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    console.Print("\x1B[1;32m[  OK  ]\x1B[0m Virtual disk     : ");
    // Note: filesystem.Exists accepts a virtual path.
    // In boot, we check if virtual root "/" exists, which translates physically to "C:/tiny_vm".
    console.Print(filesystem.Exists("/") ? "mounted\n" : "not found\n");
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    console.Print("\x1B[1;32m[  OK  ]\x1B[0m Status           : Ready\n\n");
    std::cout.flush();

    console.Clear();
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void TinyKernel::Shutdown()
{
    console.Print("TinyKernal shutdown complete.\n");
    g_Kernel = nullptr;
}
