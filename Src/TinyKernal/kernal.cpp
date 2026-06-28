#include "kernal.h"
#include <iostream>
#include <fstream>
#include <string>

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

    console.Print("=====================================\n");
    console.Print(" TinyKernal Boot\n");
    console.Print("=====================================\n\n");

    console.Print("Memory page size : ");
    std::cout << memory.PageSize() << " bytes\n";

    console.Print("Virtual disk     : ");
    // Note: filesystem.Exists accepts a virtual path.
    // In boot, we check if virtual root "/" exists, which translates physically to "C:/tiny_vm".
    console.Print(filesystem.Exists("/") ? "mounted\n" : "not found\n");

    console.Print("Status           : Ready\n\n");
}

void TinyKernel::Shutdown()
{
    console.Print("TinyKernal shutdown complete.\n");
    g_Kernel = nullptr;
}
