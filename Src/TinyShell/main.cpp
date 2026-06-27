#include "shell.h"

int main()
{
    tinyvm::ShellState state = tinyvm::LoadConfig();
    tinyvm::RunShell(state);
    return 0;
}
