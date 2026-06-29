#include "shell.h"
#include "kernal.h"
#include <windows.h>
#include "resource.h"

int main()
{
    // Icon code
    HWND hwnd = GetConsoleWindow();

    HICON icon = (HICON)LoadImage(
        GetModuleHandle(nullptr),
        MAKEINTRESOURCE(IDI_APP_ICON),
        IMAGE_ICON,
        0, 0,
        LR_DEFAULTSIZE
    );

    if (icon && hwnd)
    {
        SendMessage(hwnd, WM_SETICON, ICON_BIG,   (LPARAM)icon);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)icon);
    }

    // YOUR original code starts here
    TinyKernel kernel;

    kernel.Boot();

    tinyvm::ShellState state = tinyvm::LoadConfig();
    tinyvm::RunShell(state);
    kernel.Shutdown();

    return 0;
}