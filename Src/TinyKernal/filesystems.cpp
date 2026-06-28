#include "filesystems.h"
#include <windows.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

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

void FileSystem::Initialize(const std::string& user, const std::string& host)
{
    username = user;
    hostname = host;
    virtual_root = "C:/tiny_vm";
    virtual_cwd = "/c/users/" + username;
}

std::string FileSystem::GetCwd() const
{
    return virtual_cwd;
}

bool FileSystem::SetCwd(const std::string& path)
{
    std::string physical = ResolveVirtualPath(path);
    std::error_code ec;
    if (fs::exists(physical, ec) && fs::is_directory(physical, ec))
    {
        std::string new_vpath;
        if (path == "~")
        {
            new_vpath = "/c/users/" + username;
        }
        else if (path.rfind("~/", 0) == 0)
        {
            new_vpath = "/c/users/" + username + "/" + path.substr(2);
        }
        else if (!path.empty() && path.front() == '/')
        {
            new_vpath = path;
        }
        else
        {
            if (virtual_cwd == "/")
                new_vpath = "/" + path;
            else
                new_vpath = virtual_cwd + "/" + path;
        }

        fs::path vp(new_vpath);
        new_vpath = vp.lexically_normal().generic_string();

        if (new_vpath.empty() || new_vpath.front() != '/')
        {
            new_vpath = "/" + new_vpath;
        }

        virtual_cwd = new_vpath;
        return true;
    }
    return false;
}

std::string FileSystem::GetUsername() const
{
    return username;
}

std::string FileSystem::GetHostname() const
{
    return hostname;
}

std::string FileSystem::ResolveVirtualPath(const std::string& virtual_path) const
{
    fs::path base;
    std::string vpath = virtual_path;

    std::replace(vpath.begin(), vpath.end(), '\\', '/');

    if (vpath == "~")
    {
        base = fs::path(virtual_root) / "c" / "users" / username;
    }
    else if (vpath.rfind("~/", 0) == 0)
    {
        base = fs::path(virtual_root) / "c" / "users" / username / vpath.substr(2);
    }
    else if (!vpath.empty() && vpath.front() == '/')
    {
        base = fs::path(virtual_root) / vpath.substr(1);
    }
    else
    {
        fs::path resolved_cwd = fs::path(virtual_root) / (virtual_cwd.front() == '/' ? virtual_cwd.substr(1) : virtual_cwd);
        base = resolved_cwd / vpath;
    }

    fs::path normalized = base.lexically_normal();
    fs::path root_path = fs::path(virtual_root).lexically_normal();

    auto is_subpath = [](const fs::path& path, const fs::path& prefix) {
        auto pathIt = path.begin();
        auto prefixIt = prefix.begin();
        for (; prefixIt != prefix.end(); ++prefixIt, ++pathIt) {
            if (pathIt == path.end() || *pathIt != *prefixIt) return false;
        }
        return true;
    };

    if (normalized == root_path || is_subpath(normalized, root_path))
    {
        return normalized.generic_string();
    }
    else
    {
        return root_path.generic_string();
    }
}

std::string FileSystem::FormatVirtualPath(const std::string& vpath) const
{
    std::string home_vpath = "/c/users/" + username;
    if (vpath == home_vpath)
    {
        return "~";
    }

    if (vpath.rfind(home_vpath + "/", 0) == 0)
    {
        return "~" + vpath.substr(home_vpath.length());
    }

    return vpath;
}

bool FileSystem::CreateFile(const std::string& path)
{
    std::string physical = ResolveVirtualPath(path);
    HANDLE hFile = ::CreateFileA(
        physical.c_str(),
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
    std::string physical = ResolveVirtualPath(path);
    return ::DeleteFileA(physical.c_str());
}

bool FileSystem::CreateDirectory(const std::string& path)
{
    std::string physical = ResolveVirtualPath(path);
    return ::CreateDirectoryA(physical.c_str(), nullptr);
}

bool FileSystem::DeleteDirectory(const std::string& path)
{
    std::string physical = ResolveVirtualPath(path);
    return ::RemoveDirectoryA(physical.c_str());
}

bool FileSystem::Exists(const std::string& path)
{
    std::string physical = ResolveVirtualPath(path);
    return fs::exists(physical);
}

bool FileSystem::IsDirectory(const std::string& path)
{
    std::string physical = ResolveVirtualPath(path);
    std::error_code ec;
    return fs::is_directory(physical, ec);
}

bool FileSystem::IsFile(const std::string& path)
{
    std::string physical = ResolveVirtualPath(path);
    std::error_code ec;
    return fs::is_regular_file(physical, ec);
}

std::string FileSystem::ReadFile(const std::string& path)
{
    std::string physical = ResolveVirtualPath(path);
    std::ifstream file(physical);

    if (!file.is_open())
        return "";

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

bool FileSystem::WriteFile(const std::string& path, const std::string& data)
{
    std::string physical = ResolveVirtualPath(path);
    std::ofstream file(physical);

    if (!file.is_open())
        return false;

    file << data;
    return true;
}

bool FileSystem::Copy(const std::string& source, const std::string& destination)
{
    std::string phys_src = ResolveVirtualPath(source);
    std::string phys_dest = ResolveVirtualPath(destination);
    try
    {
        fs::copy(phys_src, phys_dest,
                 fs::copy_options::overwrite_existing | fs::copy_options::recursive);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool FileSystem::Move(const std::string& source, const std::string& destination)
{
    std::string phys_src = ResolveVirtualPath(source);
    std::string phys_dest = ResolveVirtualPath(destination);
    try
    {
        fs::rename(phys_src, phys_dest);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

std::vector<std::string> FileSystem::List(const std::string& path, std::string& error_msg)
{
    std::vector<std::string> entries;
    std::string physical = ResolveVirtualPath(path);

    std::error_code error;
    if (!fs::exists(physical, error) || !fs::is_directory(physical, error))
    {
        error_msg = "no such directory";
        return {};
    }

    for (const auto& entry : fs::directory_iterator(physical, error))
    {
        if (error)
        {
            error_msg = error.message();
            return {};
        }

        std::string name = entry.path().filename().generic_string();
        if (entry.is_directory(error))
        {
            name += "/";
        }
        entries.push_back(name);
    }

    std::sort(entries.begin(), entries.end());
    return entries;
}
