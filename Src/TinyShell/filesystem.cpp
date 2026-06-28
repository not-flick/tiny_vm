#include "filesystem.h"
#include "syscall.h"
#include <algorithm>
#include <sstream>
#include <vector>

namespace tinyvm
{
    std::string FormatShellPath(const ShellState& state, const std::string& path)
    {
        std::string home = "/c/users/" + state.username;
        if (path == home)
        {
            return "~";
        }
        if (path.rfind(home + "/", 0) == 0)
        {
            return "~" + path.substr(home.length());
        }
        return path;
    }

    FileSystem::FileSystem(const ShellState& state)
        : state(state)
    {
    }

    bool FileSystem::Exists(const std::string& path) const
    {
        auto res = Syscall(SyscallID::FILE_EXISTS, {path});
        SetError(res.error);
        return res.success && !res.data.empty() && res.data[0] == "true";
    }

    bool FileSystem::IsDirectory(const std::string& path) const
    {
        auto res = Syscall(SyscallID::IS_DIRECTORY, {path});
        SetError(res.error);
        return res.success && !res.data.empty() && res.data[0] == "true";
    }

    bool FileSystem::IsFile(const std::string& path) const
    {
        auto res = Syscall(SyscallID::IS_FILE, {path});
        SetError(res.error);
        return res.success && !res.data.empty() && res.data[0] == "true";
    }

    bool FileSystem::MkDir(const std::string& path) const
    {
        auto res = Syscall(SyscallID::CREATE_DIRECTORY, {path});
        SetError(res.error);
        return res.success;
    }

    bool FileSystem::RmDir(const std::string& path) const
    {
        auto res = Syscall(SyscallID::DELETE_DIRECTORY, {path});
        SetError(res.error);
        return res.success;
    }

    bool FileSystem::Touch(const std::string& path) const
    {
        auto res = Syscall(SyscallID::CREATE_FILE, {path});
        SetError(res.error);
        return res.success;
    }

    bool FileSystem::RmFile(const std::string& path) const
    {
        auto res = Syscall(SyscallID::DELETE_FILE, {path});
        SetError(res.error);
        return res.success;
    }

    bool FileSystem::Copy(const std::string& source, const std::string& destination) const
    {
        auto res = Syscall(SyscallID::COPY_FILE, {source, destination});
        SetError(res.error);
        return res.success;
    }

    bool FileSystem::Move(const std::string& source, const std::string& destination) const
    {
        auto res = Syscall(SyscallID::MOVE_FILE, {source, destination});
        SetError(res.error);
        return res.success;
    }

    bool FileSystem::Rename(const std::string& source, const std::string& destination) const
    {
        auto res = Syscall(SyscallID::MOVE_FILE, {source, destination});
        SetError(res.error);
        return res.success;
    }

    std::string FileSystem::ReadFile(const std::string& path) const
    {
        auto res = Syscall(SyscallID::READ_FILE, {path});
        SetError(res.error);
        return res.success && !res.data.empty() ? res.data[0] : "";
    }

    bool FileSystem::WriteFile(const std::string& path, const std::string& content) const
    {
        auto res = Syscall(SyscallID::WRITE_FILE, {path, content});
        SetError(res.error);
        return res.success;
    }

    std::vector<std::string> FileSystem::List(const std::string& path) const
    {
        auto res = Syscall(SyscallID::LIST_DIRECTORY, {path});
        SetError(res.error);
        return res.success ? res.data : std::vector<std::string>{};
    }

    std::string FileSystem::Absolute(const std::string& path) const
    {
        return Resolve(path);
    }

    std::string FileSystem::Resolve(const std::string& path) const
    {
        if (path.empty())
        {
            return state.cwd;
        }

        std::string value = path;
        std::replace(value.begin(), value.end(), '\\', '/');

        std::string resolved;
        std::string home = "/c/users/" + state.username;

        if (value == "~")
        {
            resolved = home;
        }
        else if (value.rfind("~/", 0) == 0)
        {
            resolved = home + "/" + value.substr(2);
        }
        else if (value.front() == '/')
        {
            resolved = value;
        }
        else
        {
            if (state.cwd == "/")
                resolved = "/" + value;
            else
                resolved = state.cwd + "/" + value;
        }

        std::vector<std::string> parts;
        std::stringstream ss(resolved);
        std::string item;
        while (std::getline(ss, item, '/'))
        {
            if (item.empty() || item == ".") continue;
            if (item == "..")
            {
                if (!parts.empty()) parts.pop_back();
            }
            else
            {
                parts.push_back(item);
            }
        }

        std::string normal_path = "";
        for (const auto& p : parts)
        {
            normal_path += "/" + p;
        }
        if (normal_path.empty()) normal_path = "/";
        return normal_path;
    }

    std::string FileSystem::LastError() const
    {
        return lastError;
    }

    void FileSystem::SetError(const std::string& message) const
    {
        lastError = message;
    }
}
