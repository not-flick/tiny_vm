#include "filesystem.h"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

namespace tinyvm
{
    namespace
    {
        std::string ToForwardSlashes(std::string text)
        {
            std::replace(text.begin(), text.end(), '\\', '/');
            return text;
        }

        bool StartsWithPath(const fs::path& path, const fs::path& prefix)
        {
            auto pathIt = path.begin();
            auto prefixIt = prefix.begin();

            for (; prefixIt != prefix.end(); ++prefixIt, ++pathIt)
            {
                if (pathIt == path.end() || *pathIt != *prefixIt)
                {
                    return false;
                }
            }

            return true;
        }
    }

    fs::path ResolvePath(const ShellState& state, const std::string& input)
    {
        if (input.empty())
        {
            return state.cwd.lexically_normal();
        }

        std::string value = ToForwardSlashes(input);
        fs::path resolved;

        if (value == "~")
        {
            resolved = state.home;
        }
        else if (value.rfind("~/", 0) == 0)
        {
            resolved = state.home / value.substr(2);
        }
        else if (value.front() == '/')
        {
            resolved = state.root / value.substr(1);
        }
        else
        {
            resolved = state.cwd / value;
        }

        resolved = resolved.lexically_normal();

        if (!IsInsideRoot(state, resolved))
        {
            return state.root;
        }

        return resolved;
    }

    std::string FormatShellPath(const ShellState& state, const fs::path& path)
    {
        fs::path normalized = path.lexically_normal();
        fs::path home = state.home.lexically_normal();
        fs::path root = state.root.lexically_normal();

        if (normalized == home)
        {
            return "~";
        }

        if (StartsWithPath(normalized, home))
        {
            fs::path relative = normalized.lexically_relative(home);
            return "~/" + ToForwardSlashes(relative.generic_string());
        }

        if (normalized == root)
        {
            return "/";
        }

        fs::path relative = normalized.lexically_relative(root);
        std::string display = ToForwardSlashes(relative.generic_string());
        if (display.empty() || display == ".")
        {
            return "/";
        }

        return "/" + display;
    }

    bool IsInsideRoot(const ShellState& state, const fs::path& path)
    {
        fs::path normalized = path.lexically_normal();
        fs::path root = state.root.lexically_normal();
        return normalized == root || StartsWithPath(normalized, root);
    }

    FileSystem::FileSystem(const ShellState& state)
        : state(state)
    {
    }

    bool FileSystem::Exists(const std::string& path) const
    {
        std::error_code error;
        const bool exists = fs::exists(Resolve(path), error);
        SetError(error ? error.message() : "");
        return exists && !error;
    }

    bool FileSystem::IsDirectory(const std::string& path) const
    {
        std::error_code error;
        const bool directory = fs::is_directory(Resolve(path), error);
        SetError(error ? error.message() : "");
        return directory && !error;
    }

    bool FileSystem::IsFile(const std::string& path) const
    {
        std::error_code error;
        const bool file = fs::is_regular_file(Resolve(path), error);
        SetError(error ? error.message() : "");
        return file && !error;
    }

    bool FileSystem::CreateDirectory(const std::string& path) const
    {
        std::error_code error;
        const bool created = fs::create_directory(Resolve(path), error);
        SetError(error ? error.message() : "");
        return created && !error;
    }

    bool FileSystem::RemoveDirectory(const std::string& path) const
    {
        std::error_code error;
        const fs::path resolved = Resolve(path);
        if (!fs::is_directory(resolved, error))
        {
            SetError(error ? error.message() : "not a directory");
            return false;
        }

        const bool removed = fs::remove(resolved, error);
        SetError(error ? error.message() : "");
        return removed && !error;
    }

    bool FileSystem::CreateFile(const std::string& path) const
    {
        const fs::path resolved = Resolve(path);
        std::error_code error;
        if (fs::is_directory(resolved, error))
        {
            SetError("is a directory");
            return false;
        }

        std::ofstream file(resolved, std::ios::app);
        SetError(file ? "" : "could not create file");
        return static_cast<bool>(file);
    }

    bool FileSystem::RemoveFile(const std::string& path) const
    {
        std::error_code error;
        const fs::path resolved = Resolve(path);
        if (!fs::is_regular_file(resolved, error))
        {
            SetError(error ? error.message() : "not a file");
            return false;
        }

        const bool removed = fs::remove(resolved, error);
        SetError(error ? error.message() : "");
        return removed && !error;
    }

    bool FileSystem::Copy(const std::string& source, const std::string& destination) const
    {
        std::error_code error;
        fs::copy(Resolve(source), Resolve(destination), fs::copy_options::recursive, error);
        SetError(error ? error.message() : "");
        return !error;
    }

    bool FileSystem::Move(const std::string& source, const std::string& destination) const
    {
        return Rename(source, destination);
    }

    bool FileSystem::Rename(const std::string& source, const std::string& destination) const
    {
        std::error_code error;
        fs::rename(Resolve(source), Resolve(destination), error);
        SetError(error ? error.message() : "");
        return !error;
    }

    std::string FileSystem::ReadFile(const std::string& path) const
    {
        std::ifstream file(Resolve(path));
        if (!file)
        {
            SetError("could not read file");
            return {};
        }

        std::ostringstream contents;
        contents << file.rdbuf();
        SetError("");
        return contents.str();
    }

    bool FileSystem::WriteFile(const std::string& path, const std::string& content) const
    {
        std::ofstream file(Resolve(path));
        if (!file)
        {
            SetError("could not write file");
            return false;
        }

        file << content;
        SetError(file ? "" : "could not write file");
        return static_cast<bool>(file);
    }

    std::vector<std::string> FileSystem::List(const std::string& path) const
    {
        std::vector<std::string> entries;
        const fs::path resolved = path.empty() ? state.cwd : Resolve(path);

        std::error_code error;
        for (const auto& entry : fs::directory_iterator(resolved, error))
        {
            if (error)
            {
                SetError(error.message());
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
        SetError(error ? error.message() : "");
        return entries;
    }

    std::string FileSystem::Absolute(const std::string& path) const
    {
        return Resolve(path).generic_string();
    }

    fs::path FileSystem::Resolve(const std::string& path) const
    {
        return ResolvePath(state, path);
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
