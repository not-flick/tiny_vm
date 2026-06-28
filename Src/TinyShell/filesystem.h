#pragma once

#include "shell.h"

#include <string>
#include <vector>

namespace tinyvm
{
    class FileSystem
    {
    public:
        explicit FileSystem(const ShellState& state);

        bool Exists(const std::string& path) const;
        bool IsDirectory(const std::string& path) const;
        bool IsFile(const std::string& path) const;

        bool MkDir(const std::string& path) const;
        bool RmDir(const std::string& path) const;

        bool Touch(const std::string& path) const;
        bool RmFile(const std::string& path) const;

        bool Copy(const std::string& source, const std::string& destination) const;
        bool Move(const std::string& source, const std::string& destination) const;
        bool Rename(const std::string& source, const std::string& destination) const;

        std::string ReadFile(const std::string& path) const;
        bool WriteFile(const std::string& path, const std::string& content) const;

        std::vector<std::string> List(const std::string& path) const;

        std::string Absolute(const std::string& path) const;
        std::string Resolve(const std::string& path) const;
        std::string LastError() const;

    private:
        const ShellState& state;
        mutable std::string lastError;

        void SetError(const std::string& message) const;
    };

    std::string FormatShellPath(const ShellState& state, const std::string& path);
}
