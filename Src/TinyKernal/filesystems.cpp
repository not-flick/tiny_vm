#include "filesystems.h"
#include <windows.h>
#include <fstream>
#include <sstream>
#include <filesystem>

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

bool FileSystem::CreateFile(const std::string& path)
{
    HANDLE hFile = ::CreateFileA(
        path.c_str(),
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
        return false;

    CloseHandle(hFile);
    return true;
}

bool FileSystem::DeleteFile(const std::string& path)
{
    return ::DeleteFileA(path.c_str());
}

bool FileSystem::CreateDirectory(const std::string& path)
{
    return ::CreateDirectoryA(path.c_str(), nullptr);
}

bool FileSystem::DeleteDirectory(const std::string& path)
{
    return ::RemoveDirectoryA(path.c_str());
}

bool FileSystem::Exists(const std::string& path)
{
    return fs::exists(path);
}

std::string FileSystem::ReadFile(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
        return "";

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

bool FileSystem::WriteFile(const std::string& path, const std::string& data)
{
    std::ofstream file(path);

    if (!file.is_open())
        return false;

    file << data;
    return true;
}

bool FileSystem::Copy(const std::string& source, const std::string& destination)
{
    try
    {
        fs::copy(source, destination,
                 fs::copy_options::overwrite_existing);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool FileSystem::Move(const std::string& source, const std::string& destination)
{
    try
    {
        fs::rename(source, destination);
        return true;
    }
    catch (...)
    {
        return false;
    }
}
