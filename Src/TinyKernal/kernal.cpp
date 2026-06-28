#include "kernal.h"

#include <iostream>

void TinyKernel::Boot()
{
    console.Print("=====================================\n");
    console.Print(" TinyKernal Boot\n");
    console.Print("=====================================\n\n");

    console.Print("Memory page size : ");
    std::cout << memory.PageSize() << " bytes\n";

    console.Print("Virtual disk     : ");
    console.Print(filesystem.Exists("C:/tiny_vm") ? "mounted\n" : "not found\n");

    console.Print("Status           : Ready\n\n");
}

void TinyKernel::Shutdown()
{
    console.Print("TinyKernal shutdown complete.\n");
}

