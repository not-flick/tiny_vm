#pragma once

#include <string>
#include <vector>

#ifdef CreateFile
#undef CreateFile
#endif

#ifdef DeleteFile
#undef DeleteFile
#endif

#ifdef CreateDirectory
#undef CreateDirectory
#endif

class FileSystem
{
public:
    void Initialize(const std::string& user, const std::string& host);
    std::string GetCwd() const;
    bool SetCwd(const std::string& path);
    std::string GetUsername() const;
    std::string GetHostname() const;

    std::string ResolveVirtualPath(const std::string& virtual_path) const;
    std::string FormatVirtualPath(const std::string& vpath) const;

    bool CreateFile(const std::string& path);
    bool DeleteFile(const std::string& path);

    bool CreateDirectory(const std::string& path);
    bool DeleteDirectory(const std::string& path);

    bool Exists(const std::string& path);
    bool IsDirectory(const std::string& path);
    bool IsFile(const std::string& path);

    std::string ReadFile(const std::string& path);
    bool WriteFile(const std::string& path, const std::string& data);

    bool Copy(const std::string& source, const std::string& destination);
    bool Move(const std::string& source, const std::string& destination);

    std::vector<std::string> List(const std::string& path, std::string& error_msg);

private:
    std::string virtual_cwd{"/"};
    std::string virtual_root{"C:/tiny_vm"};
    std::string username{"user"};
    std::string hostname{"tinyvm"};
};
