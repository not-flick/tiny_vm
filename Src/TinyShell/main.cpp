#include "shell.h"
#include "kernal.h"

int main()
{
    TinyKernel kernel;
    kernel.Boot();

    tinyvm::ShellState state = tinyvm::LoadConfig();
    tinyvm::RunShell(state);

    kernel.Shutdown();
    return 0;
}
