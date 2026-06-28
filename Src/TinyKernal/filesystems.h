#pragma once

#include <string>

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
    bool CreateFile(const std::string& path);
    bool DeleteFile(const std::string& path);

    bool CreateDirectory(const std::string& path);
    bool DeleteDirectory(const std::string& path);

    bool Exists(const std::string& path);

    std::string ReadFile(const std::string& path);
    bool WriteFile(const std::string& path, const std::string& data);

    bool Copy(const std::string& source, const std::string& destination);
    bool Move(const std::string& source, const std::string& destination);
};
