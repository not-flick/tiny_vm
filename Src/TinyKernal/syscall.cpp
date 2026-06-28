#include "syscall.h"
#include "kernal.h"
#include "filesystems.h"

#include <cstdlib>
#include <exception>
#include <filesystem>
#include <windows.h>
#include <conio.h>
#include <io.h>
#include <iostream>
#include <fstream>

#ifdef CreateFile
#undef CreateFile
#endif

#ifdef DeleteFile
#undef DeleteFile
#endif

#ifdef CreateDirectory
#undef CreateDirectory
#endif

namespace fs = std::filesystem;

TinyKernel* g_Kernel = nullptr;

SyscallResult Syscall(SyscallID id, const std::vector<std::string>& args)
{
    if (!g_Kernel)
    {
        return {false, "Kernel not booted", {}};
    }
    SyscallDispatcher dispatcher(*g_Kernel);
    return dispatcher.Handle(id, args);
}

SyscallDispatcher::SyscallDispatcher(TinyKernel& kernel)
    : kernel(kernel)
{
}

SyscallResult SyscallDispatcher::Handle(
    SyscallID id,
    const std::vector<std::string>& args)
{
    SyscallResult result;

    switch (id)
    {
        case SyscallID::PRINT:
        {
            if (!args.empty())
            {
                kernel.console.Print(args[0]);
            }
            result.success = true;
            return result;
        }

        case SyscallID::CLEAR:
        {
            kernel.console.Clear();
            result.success = true;
            return result;
        }

        case SyscallID::CONSOLE_INIT:
        {
            SetConsoleOutputCP(CP_UTF8);
            SetConsoleCP(CP_UTF8);
            if (!args.empty())
            {
                SetConsoleTitleA(args[0].c_str());
            }
            else
            {
                SetConsoleTitleA("TinyShell");
            }

            std::wstring path(MAX_PATH, L'\0');
            const DWORD length = GetModuleFileNameW(nullptr, path.data(), static_cast<DWORD>(path.size()));
            if (length != 0)
            {
                path.resize(length);
                fs::path iconPath = fs::path(path).parent_path() / "TinyShell.ico";
                std::error_code error;
                if (fs::exists(iconPath, error))
                {
                    const std::wstring icon = iconPath.wstring();
                    HICON bigIcon = static_cast<HICON>(LoadImageW(nullptr, icon.c_str(), IMAGE_ICON, 32, 32, LR_LOADFROMFILE));
                    HICON smallIcon = static_cast<HICON>(LoadImageW(nullptr, icon.c_str(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE));
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

                    if (std::getenv("WT_SESSION") != nullptr)
                    {
                        std::cout << "\x1b]9;9;" << iconPath.generic_string() << "\x07";
                        std::cout.flush();
                    }
                }
            }

            result.success = true;
            return result;
        }

        case SyscallID::CONSOLE_IS_ATTY:
        {
            bool is_tty = _isatty(_fileno(stdin)) != 0;
            result.success = true;
            result.data.push_back(is_tty ? "true" : "false");
            return result;
        }

        case SyscallID::CONSOLE_READ_CHAR:
        {
            int ch = _getch();
            result.success = true;
            result.data.push_back(std::to_string(ch));
            return result;
        }

        case SyscallID::CONSOLE_READ_LINE:
        {
            std::string input;
            std::getline(std::cin, input);
            result.success = true;
            result.data.push_back(input);
            return result;
        }

        case SyscallID::CREATE_FILE:
        {
            if (!args.empty())
            {
                result.success = kernel.filesystem.CreateFile(args[0]);
                if (!result.success) result.error = "Could not create file";
            }
            else
            {
                result.error = "Missing arguments";
            }
            return result;
        }

        case SyscallID::DELETE_FILE:
        {
            if (!args.empty())
            {
                result.success = kernel.filesystem.DeleteFile(args[0]);
                if (!result.success) result.error = "Could not delete file";
            }
            else
            {
                result.error = "Missing arguments";
            }
            return result;
        }

        case SyscallID::CREATE_DIRECTORY:
        {
            if (!args.empty())
            {
                result.success = kernel.filesystem.CreateDirectory(args[0]);
                if (!result.success) result.error = "Could not create directory";
            }
            else
            {
                result.error = "Missing arguments";
            }
            return result;
        }

        case SyscallID::DELETE_DIRECTORY:
        {
            if (!args.empty())
            {
                result.success = kernel.filesystem.DeleteDirectory(args[0]);
                if (!result.success) result.error = "Could not delete directory";
            }
            else
            {
                result.error = "Missing arguments";
            }
            return result;
        }

        case SyscallID::FILE_EXISTS:
        {
            if (!args.empty())
            {
                result.success = true;
                result.data.push_back(kernel.filesystem.Exists(args[0]) ? "true" : "false");
            }
            else
            {
                result.error = "Missing arguments";
            }
            return result;
        }

        case SyscallID::IS_DIRECTORY:
        {
            if (!args.empty())
            {
                result.success = true;
                result.data.push_back(kernel.filesystem.IsDirectory(args[0]) ? "true" : "false");
            }
            else
            {
                result.error = "Missing arguments";
            }
            return result;
        }

        case SyscallID::IS_FILE:
        {
            if (!args.empty())
            {
                result.success = true;
                result.data.push_back(kernel.filesystem.IsFile(args[0]) ? "true" : "false");
            }
            else
            {
                result.error = "Missing arguments";
            }
            return result;
        }

        case SyscallID::READ_FILE:
        {
            if (!args.empty())
            {
                if (kernel.filesystem.IsDirectory(args[0]))
                {
                    result.success = false;
                    result.error = "is a directory";
                }
                else
                {
                    std::string content = kernel.filesystem.ReadFile(args[0]);
                    result.success = true; // ReadFile returns empty string on failure, but we'll treat it as success or handled
                    result.data.push_back(content);
                }
            }
            else
            {
                result.error = "Missing arguments";
            }
            return result;
        }

        case SyscallID::WRITE_FILE:
        {
            if (args.size() >= 2)
            {
                result.success = kernel.filesystem.WriteFile(args[0], args[1]);
                if (!result.success) result.error = "Could not write file";
            }
            else
            {
                result.error = "Missing arguments";
            }
            return result;
        }

        case SyscallID::LIST_DIRECTORY:
        {
            if (!args.empty())
            {
                std::string err;
                std::vector<std::string> entries = kernel.filesystem.List(args[0], err);
                if (!err.empty())
                {
                    result.success = false;
                    result.error = err;
                }
                else
                {
                    result.success = true;
                    result.data = entries;
                }
            }
            else
            {
                result.error = "Missing arguments";
            }
            return result;
        }

        case SyscallID::COPY_FILE:
        {
            if (args.size() >= 2)
            {
                result.success = kernel.filesystem.Copy(args[0], args[1]);
                if (!result.success) result.error = "Could not copy file";
            }
            else
            {
                result.error = "Missing arguments";
            }
            return result;
        }

        case SyscallID::MOVE_FILE:
        {
            if (args.size() >= 2)
            {
                result.success = kernel.filesystem.Move(args[0], args[1]);
                if (!result.success) result.error = "Could not move file";
            }
            else
            {
                result.error = "Missing arguments";
            }
            return result;
        }

        case SyscallID::GET_CWD:
        {
            result.success = true;
            result.data.push_back(kernel.filesystem.GetCwd());
            return result;
        }

        case SyscallID::SET_CWD:
        {
            if (!args.empty())
            {
                result.success = kernel.filesystem.SetCwd(args[0]);
                if (!result.success) result.error = "No such directory";
            }
            else
            {
                result.error = "Missing arguments";
            }
            return result;
        }

        case SyscallID::GET_USERNAME:
        {
            result.success = true;
            result.data.push_back(kernel.filesystem.GetUsername());
            return result;
        }

        case SyscallID::GET_HOSTNAME:
        {
            result.success = true;
            result.data.push_back(kernel.filesystem.GetHostname());
            return result;
        }

        case SyscallID::RUN_PROCESS:
        {
            if (!args.empty())
            {
                kernel.process.Launch(args[0]);
                result.success = true;
                return result;
            }
            result.error = "Missing arguments";
            return result;
        }

        case SyscallID::EXIT:
        {
            int code = 0;
            if (!args.empty())
            {
                try
                {
                    code = std::stoi(args[0]);
                }
                catch (const std::exception&)
                {
                }
            }
            kernel.process.Exit(code);
            result.success = true;
            return result;
        }

        default:
            result.error = "Unknown syscall ID";
            return result;
    }
}
