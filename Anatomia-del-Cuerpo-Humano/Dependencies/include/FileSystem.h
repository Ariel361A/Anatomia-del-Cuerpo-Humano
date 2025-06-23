#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

class FileSystem
{
private:
    typedef std::string(*Builder)(const std::string& path);

public:
    static std::string getPath(const std::string& path)
    {
        static Builder pathBuilder = getPathBuilder();
        return (*pathBuilder)(path);
    }

private:
    static const std::string& getRoot();
    static std::string getPathRelativeRoot(const std::string& path);
    static std::string getPathRelativeBinary(const std::string& path);
    static Builder getPathBuilder();
};

#endif // FILESYSTEM_H
